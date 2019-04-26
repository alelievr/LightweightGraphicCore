#include <memory>
#include <unistd.h>

#include "Core/Application.hpp"
#include "Core/Rendering/ForwardRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Time.hpp"
#include "IncludeDeps.hpp"
#include "Core/Vulkan/ProfilingSample.hpp"
#include "Core/Profiler.hpp"

// Volk function definitions + init
#include VOLK_SOURCE

using namespace LWGC;

Application *				Application::_app = nullptr;
Delegate< void(void) >		Application::update;
Delegate< void(void) >		Application::lateUpdate;

Application::Application(bool initDeafultRenderPipeline) : _window(nullptr), hierarchy(std::make_shared< Hierarchy >()), _shouldNotQuit(true)
{
	if (initDeafultRenderPipeline)
	{
		RenderPipelineManager::SetCurrentRenderPipeline(new ForwardRenderPipeline());
	}
	_app = this;
}

Application::~Application(void)
{
	_materialTable.DestroyObjects();
	_textureTable.DestroyObjects();
	Vk::Release();

	RenderPipelineManager::ReleaseAllPipelines();

	if (_window != NULL)
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

void			ErrorCallback(int err, const char * description)
{
	std::cout << "GLFW error[" << err << "]: " << description << std::endl;
}

void			Application::Init(const AppCapability capabilities) noexcept
{
	glfwSetErrorCallback(ErrorCallback);
	glfwInit();

	Vk::CheckResult(volkInitialize(), "Can't initialize volk");

	std::vector< std::string > deviceExtensions = {
#ifdef DEBUG
		VK_EXT_DEBUG_MARKER_EXTENSION_NAME, // TODO: enable the extension in the vulkan layer
#endif
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME
	};

#ifdef __unix__
	_instance.SetValidationLayers({
		"VK_LAYER_LUNARG_standard_validation",
	});
#endif

	// TODO: std::map capabilities to extensions
	if ((int)capabilities & (int)AppCapability::RayTracing)
		deviceExtensions.push_back(VK_NV_RAY_TRACING_EXTENSION_NAME);
	if ((int)capabilities & (int)AppCapability::CooperativeMatrices)
		deviceExtensions.push_back(VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME);

	_instance.SetDeviceExtensions(deviceExtensions);
	_instance.SetApplicationName("LWGC"); // This should be the application name but for the moment we'll keep it like this

	_instance.Initialize();
}

bool			Application::ShouldNotQuit(void) const noexcept
{
	return this->_shouldNotQuit;
}

void			Application::Quit(void) noexcept
{
	glfwSetWindowShouldClose(_window, true);
}

void		Application::FramebufferResizeCallback(GLFWwindow *window, int width, int height)
{
	// Avoid getting stuck with 0-sized windows
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	auto pipeline = reinterpret_cast< RenderPipeline * >(glfwGetWindowUserPointer(window));
	pipeline->framebufferResized = true;
}

void			Application::UpdateRenderPipeline(void)
{
	// By setting this to null we ensure that the first time the function is called,
	// we initialize the materials using the renderpipeline (is there is one)
	static RenderPipeline *	lastFramePipeline = nullptr;
	auto currentPipe = RenderPipelineManager::currentRenderPipeline;

	if (currentPipe != nullptr)
	{
		// Initialize the pipeline
		if (!currentPipe->IsInitialized())
		{
			currentPipe->Initialize(&_swapChain);
			currentPipe->CreateSyncObjects();
		}

		// Check if the pipeline have changed since the last frame
		if (lastFramePipeline != currentPipe)
		{
			// In this case, we need to recreate all materials
			_materialTable.SetRenderPass(currentPipe->GetRenderPass());

			if (!_materialTable.IsInitialized())
				_materialTable.Initialize(&_swapChain, currentPipe->GetRenderPass());

			// We normaly need to upadte the renderpass of the materials if we change a pipeline
			// else
				// _materialTable.RecreateAll();

			// TODO: Recreate ImGUI materials too
			_imGUI.UpdatePipelineDependentDatas();

			glfwSetWindowUserPointer(_window, currentPipe);
			lastFramePipeline = currentPipe;
		}
	}
}

void			Application::Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept
{
	if (!glfwVulkanSupported())
	{
		std::cout << "Vulkan is not supported :(" << std::endl << "exiting..." << std::endl;
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, (flags & WindowFlag::Resizable) != 0);
	glfwWindowHint(GLFW_DECORATED, (flags & WindowFlag::Decorated) != 0);
	glfwWindowHint(GLFW_FOCUSED, (flags & WindowFlag::Focused) != 0);
	glfwWindowHint(GLFW_FLOATING, (flags & WindowFlag::Floating) != 0);
	glfwWindowHint(GLFW_MAXIMIZED, (flags & WindowFlag::Maximized) != 0);

	_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

	_eventSystem.BindWindow(_window);

	try {
		_surface.Initialize(_window);
		_swapChain.Initialize(_surface);

		// Vk needs logical device (which require surface for creation (due to swapchain support checks))
		Vk::Initialize();
		Time::Initialize();

		_imGUI.Initialize(&_swapChain, &_surface);
		UpdateRenderPipeline();

		hierarchy->Initialize();
	} catch (const std::runtime_error & e) {
		std::cout << "Error while initializing the render pipeline:" << std::endl << e.what() << std::endl;
		exit(-1);
	} catch (const std::runtime_error * e) { // for C# syntax raising exceptions
		std::cout << "Error while initializing the render pipeline:" << std::endl << e->what() << std::endl;
		exit(-1);
	} catch (...) {
		std::cout << "Unknown error while initializing the render pipeline !" << std::endl;
	}

	glfwSetFramebufferSizeCallback(_window, FramebufferResizeCallback);
}
#include <limits>

void				Application::Update(void) noexcept
{
	UpdateRenderPipeline();

	glfwPollEvents();
	Time::BeginFrame();
	Application::update.Invoke();
	Application::lateUpdate.Invoke();

	//TODO: hierarchy get cameras
	const auto cameras = hierarchy->GetCameras();

	auto currentPipe = RenderPipelineManager::currentRenderPipeline;

	Profiler::Reset();

	if (currentPipe != nullptr)
	{
		auto sample = ProfilingSample("Frame");

		// If rendering was successful, we can draw the GUI and present the frame
		if (currentPipe->RenderInternal(cameras, hierarchy->GetRenderContext()))
		{
			_imGUI.BeginFrame();
			currentPipe->RenderGUI(hierarchy->GetRenderContext());
			_imGUI.EndFrame();

			currentPipe->PresentFrame();
		}
	}
	else // When there is no pipeline graphic, we limit the application framerate to 60
		usleep((1.0f / 60.0f) * 1000000.0f);

	_shouldNotQuit = !glfwWindowShouldClose(_window);
}

SwapChain *			Application::GetSwapChain(void) noexcept { return &this->_swapChain; }
EventSystem *		Application::GetEventSystem(void) noexcept { return &this->_eventSystem; }
Hierarchy *			Application::GetHierarchy(void) noexcept { return this->hierarchy.get(); }
MaterialTable *		Application::GetMaterialTable(void) noexcept { return &this->_materialTable; }
TextureTable *		Application::GetTextureTable(void) noexcept { return &this->_textureTable; }

Application *	Application::Get(void) noexcept { return _app; }

std::ostream &	operator<<(std::ostream & o, Application const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
