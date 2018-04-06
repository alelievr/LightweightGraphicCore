#include "Application.hpp"

#include "ForwardRenderPipeline.hpp"

using namespace LWGE;

Application::Application(void) : _shouldNotQuit(true)
{
	std::cout << "Default constructor of Application called" << std::endl;
	this->_renderPipeline = new ForwardRenderPipeline();
}

Application::~Application(void)
{
	std::cout << "Destructor of Application called" << std::endl;
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
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, (flags | WindowFlag::Resizable) != 0);
	glfwWindowHint(GLFW_DECORATED, (flags & WindowFlag::Decorated) != 0);
	glfwWindowHint(GLFW_FOCUSED, (flags & WindowFlag::Focused) != 0);
	glfwWindowHint(GLFW_FLOATING, (flags & WindowFlag::Floating) != 0);
	glfwWindowHint(GLFW_MAXIMIZED, (flags & WindowFlag::Maximized) != 0);

	_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
}

IRenderPipeline *	Application::GetRenderPipeline(void) const noexcept { return (this->_renderPipeline); }
void				Application::SetRenderPipeline(IRenderPipeline * tmp) noexcept { this->_renderPipeline = tmp; }

const EventSystem *	Application::GetEventSystem(void) const noexcept { return &(this->_eventSystem); }

std::ostream &	operator<<(std::ostream & o, Application const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
