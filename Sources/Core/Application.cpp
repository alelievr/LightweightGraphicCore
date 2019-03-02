#include <memory>

#include "Core/Application.hpp"
#include "Core/Rendering/ForwardRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Time.hpp"

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

void			Application::Init(void) noexcept
{
	glfwSetErrorCallback(ErrorCallback);
	glfwInit();

#ifdef __unix__
	_instance.SetValidationLayers({
		"VK_LAYER_LUNARG_standard_validation"
	});
#endif
	_instance.SetDeviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
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

	auto app = reinterpret_cast<RenderPipeline *>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
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
			else
				_materialTable.RecreateAll();

			// TODO: Recreate ImGUI materials too

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

	Time::Initialize();
	_imGUI.Initialize(&_swapChain, &_surface);
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
	currentPipe->RenderInternal(cameras, hierarchy->GetRenderContext());

	_imGUI.BeginFrame();
	currentPipe->RenderGUI(hierarchy->GetRenderContext());
	_imGUI.EndFrame();

	currentPipe->PresentFrame();

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
