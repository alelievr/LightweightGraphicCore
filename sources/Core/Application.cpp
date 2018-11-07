#include <memory>

#include "Application.hpp"
#include "Rendering/ForwardRenderPipeline.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
// TODO: move to vulkan
#include "imgui_impl_opengl3.h"

using namespace LWGC;

Application::Application(void) : _window(NULL), _shouldNotQuit(true)
{
	std::cout << "Default constructor of Application called" << std::endl;
	this->_renderPipeline = std::make_shared< ForwardRenderPipeline >();
}

Application::~Application(void)
{
	if (_window != NULL)
	{
		//TODO: change this to vulkan
		//ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		//ImGui::DestroyContext();
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
}

bool			Application::ShouldNotQuit(void) const noexcept
{
	return this->_shouldNotQuit;
}

void			Application::Quit(void) noexcept
{
	_shouldNotQuit = false;
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

//	VkSurfaceKHR surface;
//	VkResult err = glfwCreateWindowSurface(instance, _window, NULL, &surface);

	_eventSystem.BindWindow(_window);

	// Init IMGUI
/*	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// TODO: move to vulkan here
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();*/
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
