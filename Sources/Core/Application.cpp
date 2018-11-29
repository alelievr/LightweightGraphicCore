#include <memory>

#include "Application.hpp"
#include "Rendering/ForwardRenderPipeline.hpp"
#include "Vulkan/VulkanInstance.hpp"
#include "Core/Components/MeshRenderer.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

using namespace LWGC;

Application::Application(void) : _window(NULL), _hierarchy(std::make_shared< Hierarchy >()), _shouldNotQuit(true)
{
	this->_renderPipeline = new ForwardRenderPipeline();
}

Application::~Application(void)
{
	delete _renderPipeline;
	
	if (_window != NULL)
	{
		// 	ImGui_ImplVulkan_Shutdown();
		// 	ImGui_ImplGlfw_Shutdown();
		// 	ImGui::DestroyContext();
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

	_instance.SetValidationLayers({});
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
	_shouldNotQuit = false;
}

void		Application::FramebufferResizeCallback(GLFWwindow *window, int width, int height)
{
	// Avoid getting stuck with 0-sized windows
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	auto app = reinterpret_cast<VulkanRenderPipeline *>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
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

	_surface.Initialize(_window);
	_swapChain.Initialize(_surface);
	
	// Vk needs logical device (which require surface for creation (due to swapchain support checks))
	Vk::Initialize();

	try {
		_renderPipeline->Initialize(&_swapChain);
		_renderPipeline->CreateSyncObjects();
	} catch (const std::runtime_error & e) {
		std::cout << "Error while initializing the render pipeline:" << std::endl << e.what() << std::endl;
		exit(-1);
	} catch (const std::runtime_error * e) { // for C# syntax raising exceptions
		std::cout << "Error while initializing the render pipeline:" << std::endl << e->what() << std::endl;
		exit(-1);
	} catch (...) {
		std::cout << "Unknown error while initializing the render pipeline !" << std::endl;
	}
	
	_hierarchy->Initialize();
	
	glfwSetWindowUserPointer(_window, &_renderPipeline);
	glfwSetFramebufferSizeCallback(_window, FramebufferResizeCallback);

	// // Init IMGUI
	// IMGUI_CHECKVERSION();
	// ImGui::CreateContext();

	// // Is this important ?
    // ImGuiIO& io = ImGui::GetIO(); (void)io;

	// ImGui_ImplVulkan_InitInfo initInfo = {};
	// initInfo.Instance = _instance.GetInstance();
	// initInfo.PhysicalDevice = _instance.GetPhysicalDevice();
	// initInfo.Device = _instance.GetDevice();
	// initInfo.QueueFamily = _instance.GetGraphicQueueIndex();
	// initInfo.Queue = _instance.GetGraphicQueue();
	// initInfo.PipelineCache = VK_NULL_HANDLE;
	// initInfo.DescriptorPool = _material.GetDescriptorPool();
	// initInfo.Allocator = VK_NULL_HANDLE;
	// initInfo.CheckVkResultFn = Vk::CheckResult;

	// ImGui_ImplGlfw_InitForVulkan(_window, true);
	// ImGui_ImplVulkan_Init(&initInfo, _renderPass.GetRenderPass());
	// ImGui::StyleColorsDark();
}

void				Application::Update(void) noexcept
{
	glfwPollEvents();

	//TODO: hierarchy get cameras
	const auto cameras =_hierarchy->GetCameras();
	_renderPipeline->RenderInternal(cameras, _hierarchy->GetRenderContext());

/*	// Draw GUI on top of everything (after pipeline rendering)
	// TODO: move to vulkan
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

	_shouldNotQuit = !glfwWindowShouldClose(_window);
}

EventSystem *		Application::GetEventSystem(void) noexcept { return &this->_eventSystem; }
Hierarchy *			Application::GetHierarchy(void) noexcept { return this->_hierarchy.get(); }

std::ostream &	operator<<(std::ostream & o, Application const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
