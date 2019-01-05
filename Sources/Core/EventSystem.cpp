#include "EventSystem.hpp"

#include <memory>
#include "Core/Application.hpp"
#include "Core/Delegate.tpp"

using namespace LWGC;

std::map< GLFWwindow *, EventSystem * > EventSystem::eventSystems;
EventSystem *	EventSystem::eventSystemInstance;

EventSystem::EventSystem(void)
{
	eventSystemInstance = this;
	delta = glm::vec2(0, 0);
	oldMousePosition = glm::vec2(0, 0);
}

EventSystem::~EventSystem(void)
{
}

void			EventSystem::BindWindow(GLFWwindow * window)
{
	_window = window;
	eventSystems[window] = this;

	glfwSetCursorEnterCallback(window, [](GLFWwindow * window, int entered)
		{
			auto & self = eventSystems[window];
			double posX;
			double posY;
			glfwGetCursorPos(window, &posX, &posY);
			const glm::vec2 & mousePosition = glm::vec2(posX, posY);

			self->onMouseMove.Invoke(mousePosition, static_cast< MouseMoveAction >(entered));
		}
	);
	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
		{
			auto & self = eventSystems[window];
			self->onFocus.Invoke(focused);
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			auto & self = eventSystems[window];
			double posX;
			double posY;
			glfwGetCursorPos(window, &posX, &posY);

			self->onKey.Invoke(static_cast< KeyCode >(key), static_cast< ButtonAction >(action));

			(void)mods;
			(void)scancode;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow * window, int button, int action, int mods)
		{
			auto & self = eventSystems[window];
			double posX;
			double posY;
			glfwGetCursorPos(window, &posX, &posY);
			const glm::vec2 & mousePosition = glm::vec2(posX, posY);

			self->onMouseClick.Invoke(mousePosition, static_cast< ButtonAction >(action));

			(void)mods;
		}
	);
	glfwSetCursorPosCallback(window, [](GLFWwindow * window, double posX, double posY)
		{
			auto & self = eventSystems[window];
			const glm::vec2 & mousePosition = glm::vec2(posX, posY);
			
			self->delta = mousePosition - self->oldMousePosition;
			self->oldMousePosition = mousePosition;
			self->onMouseMove.Invoke(mousePosition, MouseMoveAction::Move);
		}
	);

	Application::update.AddListener([&](){
		}
	);
	
	Application::lateUpdate.AddListener([&](){
			this->delta = glm::vec2(0, 0);
		}
	);
}

void				EventSystem::LockCursor(void)
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void				EventSystem::ReleaseCursor(void)
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void				EventSystem::ToggleLockCursor(void)
{
	if (glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		ReleaseCursor();
	else
		LockCursor();
}

EventSystem *		EventSystem::Get(void)
{
	return eventSystemInstance;
}

std::ostream &	operator<<(std::ostream & o, EventSystem const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
