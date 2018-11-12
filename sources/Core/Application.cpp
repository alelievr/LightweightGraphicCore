#include <memory>

#include "Application.hpp"
#include "Rendering/ForwardRenderPipeline.hpp"
#include "Vulkan/VulkanInstance.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

using namespace LWGC;

Application::Application(void) : _window(NULL), _shouldNotQuit(true), _framebufferResized(false)
{
	std::cout << "Default constructor of Application called" << std::endl;
	this->_renderPipeline = std::make_shared< ForwardRenderPipeline >();
}

Application::~Application(void)
{
	if (_window != NULL)
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	std::cout << "Destructor of Application called" << std::endl;
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
	auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->_framebufferResized = true;
}

void			Application::Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept
{
	if (glfwVulkanSupported())
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
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, FramebufferResizeCallback);

	_eventSystem.BindWindow(_window);

	_surface.Initialize(_window);
	_swapChain.Initialize(_surface);
	CreateRenderPass();
	_material.Initialize(&_swapChain, &_renderPass);

	// Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Is this important ?
    ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = _instance.GetInstance();
	initInfo.PhysicalDevice = _instance.GetPhysicalDevice();
	initInfo.Device = _instance.GetDevice();
	initInfo.QueueFamily = _instance.GetGraphicQueueIndex();
	initInfo.Queue = _instance.GetGraphicQueue();
	initInfo.PipelineCache = VK_NULL_HANDLE;
	initInfo.DescriptorPool = _material.GetDescriptorPool();
	initInfo.Allocator = VK_NULL_HANDLE;
	initInfo.CheckVkResultFn = Vk::CheckResult;

	// TODO: move to vulkan here
	ImGui_ImplGlfw_InitForVulkan(_window, true);
	ImGui_ImplVulkan_Init(&initInfo, _renderPass.GetRenderPass());
	ImGui::StyleColorsDark();
}

void				Application::Update(void) noexcept
{
	glfwPollEvents();

	_renderPipeline->Render();

/*	// Draw GUI on top of everything (after pipeline rendering)
	// TODO: move to vulkan
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// User IMGUI calls
	_renderPipeline->RenderImGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

	//glfwSwapBuffers(_window);

	_shouldNotQuit = !glfwWindowShouldClose(_window);
}

void				Application::CreateRenderPass(void)
{
	_renderPass.Initialize(&_swapChain);

	// Currently fixed renderpass, TODO: have the hability to change the layout of the renderPass
	_renderPass.AddAttachment(
		RenderPass::GetDefaultColorAttachment(_swapChain.GetImageFormat()),
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	_renderPass.SetDepthAttachment(
		RenderPass::GetDefaultDepthAttachment(_instance.FindDepthFormat()),
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	_renderPass.AddDependency(dependency);

	// Will also generate framebuffers
	_renderPass.Create();
}

#include <memory>
IRenderPipeline &	Application::GetRenderPipeline(void) const noexcept { return *(this->_renderPipeline); }
void				Application::SetRenderPipeline(std::shared_ptr< IRenderPipeline > tmp) noexcept { this->_renderPipeline = tmp; }

EventSystem *		Application::GetEventSystem(void) noexcept { return &this->_eventSystem; }
Hierarchy *			Application::GetHierarchy(void) noexcept { return &this->_hierarchy; }

std::ostream &	operator<<(std::ostream & o, Application const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
