#include "EventSystem.hpp"

#include <memory>
#include "Core/Application.hpp"
#include "Core/Delegate.hpp"

using namespace LWGC;

std::map< GLFWwindow *, EventSystem * > EventSystem::eventSystems;
EventSystem *	EventSystem::eventSystemInstance;

EventSystem::EventSystem(void)
{
	eventSystemInstance = this;
}

EventSystem::~EventSystem(void)
{
}

	// self->onKey.Invoke(static_cast< KeyCode >(key), static_cast< KeyAction >(action));

void			EventSystem::BindWindow(GLFWwindow * window)
{
	_window = window;
	eventSystems[window] = this;
	_current = {};

	glfwSetCursorEnterCallback(window, [](GLFWwindow * window, int entered)
		{
			auto & self = eventSystems[window];

			if (self->_onMouseEnter != nullptr && entered == GLFW_TRUE)
				self->_onMouseEnter();
			else if (self->_onMouseExit != nullptr && entered == GLFW_FALSE)
				self->_onMouseExit();
		}
	);
	
	glfwSetKeyCallback(window, [](GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			auto & self = eventSystems[window];

			self->onKey.Invoke(static_cast< KeyCode >(key), static_cast< KeyAction >(action));

			(void)mods;
			(void)scancode;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow * window, int button, int action, int mods)
		{
			auto & self = eventSystems[window];
			const auto & pos = self->_current.GetMousePosition();

			if (self->_onMouseDown != nullptr && action == GLFW_PRESS)
				self->_onMouseDown(pos.x, pos.y, button);
			if (self->_onMouseUp != nullptr && action == GLFW_RELEASE)
				self->_onMouseUp(pos.x, pos.y, button);

			(void)mods;
		}
	);
	glfwSetCursorPosCallback(window, [](GLFWwindow * window, double posX, double posY)
		{
			auto & self = eventSystems[window];

			if (self->_onMouseMove != nullptr)
				self->_onMouseMove(posX, posY);
		}
	);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	_current.mousePosition.x = mouseX;
	_current.mousePosition.y = mouseY;
	
	static glm::vec2 oldMousePosition = _current.mousePosition;

	// TODO: maybe another function that habdles the event logic
	Application::update.AddListener([&](){
			// Per-frame event system internal update
			// _current.type = EventType::Ignore;
			_delta = _mousePosition - _oldMousePosition;
			_oldMousePosition = _mousePosition;
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
