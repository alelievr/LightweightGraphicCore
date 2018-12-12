#include "EventSystem.hpp"

#include <memory>
#include "Core/Application.hpp"

using namespace LWGC;

std::map< GLFWwindow *, EventSystem * > EventSystem::eventSystems;
EventSystem *	EventSystem::eventSystemInstance;

EventSystem::EventSystem(void)
{
	eventSystemInstance = this;

	this->_onQuit = nullptr;
	this->_onFocus = nullptr;

	// wtf ...
	using std::placeholders::_1;
	using std::placeholders::_2;
	using std::placeholders::_3;

	this->_onMouseMove = std::bind(&EventSystem::DefaultMouseMoveAction, this, _1, _2);
	this->_onMouseDown = std::bind(&EventSystem::DefaultMouseDownAction, this, _1, _2, _3);
	this->_onMouseUp = std::bind(&EventSystem::DefaultMouseUpAction, this, _1, _2, _3);

	this->_onKeyDown = std::bind(&EventSystem::DefaultKeyDownAction, this, _1);
	this->_onKeyUp = std::bind(&EventSystem::DefaultKeyUpAction, this, _1);
	this->_onKeyStay = nullptr;

	this->_onMouseEnter = nullptr;
	this->_onMouseExit = nullptr;
}

void EventSystem::DefaultMouseMoveAction(float x, float y)
{
	_current.SetMousePosition(x, y);
}

void EventSystem::DefaultMouseDownAction(float x, float y, int button)
{
	_current.mouseButton = button;
	_current.type = EventType::MouseDown;
	_current.SetMousePosition(x, y);
}

void EventSystem::DefaultMouseUpAction(float x, float y, int button)
{
	_current.mouseButton = button;
	_current.type = EventType::MouseUp;
	_current.SetMousePosition(x, y);
}

void EventSystem::DefaultKeyDownAction(KeyCode k)
{
	_current.type = EventType::KeyDown;
	_current.keyCode = k;
}

void EventSystem::DefaultKeyUpAction(KeyCode k)
{
	_current.type = EventType::KeyUp;
	_current.keyCode = k;
}

EventSystem::~EventSystem(void)
{
}

void			EventSystem::BindWindow(GLFWwindow * window)
{
	eventSystems[window] = this;

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

			if (self->_onKeyDown != nullptr && action == GLFW_PRESS)
				self->_onKeyDown(static_cast< KeyCode >(key));
			if (self->_onKeyUp != nullptr && action == GLFW_RELEASE)
				self->_onKeyUp(static_cast< KeyCode >(key));
			if (self->_onKeyStay != nullptr && action == GLFW_REPEAT)
				self->_onKeyStay(static_cast< KeyCode >(key));

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

	Application::update.AddListener([&](){
			// Per-frame event system internal update
			_current.delta = _current.mousePosition - oldMousePosition;
			oldMousePosition = _current.mousePosition;
		}
	);
}

EventSystem *		EventSystem::Get(void)
{
	return eventSystemInstance;
}

OnQuitCallback		EventSystem::GetOnQuit(void) const { return (this->_onQuit); }
void		EventSystem::SetOnQuit(OnQuitCallback tmp) { this->_onQuit = tmp; }

OnFocusCallback		EventSystem::GetOnFocus(void) const { return (this->_onFocus); }
void		EventSystem::SetOnFocus(OnFocusCallback tmp) { this->_onFocus = tmp; }

OnMouseMoveCallback		EventSystem::GetOnMouseMove(void) const { return (this->_onMouseMove); }
void		EventSystem::SetOnMouseMove(OnMouseMoveCallback tmp) { this->_onMouseMove = tmp; }

OnMouseDownCallback		EventSystem::GetOnMouseDown(void) const { return (this->_onMouseDown); }
void		EventSystem::SetOnMouseDown(OnMouseDownCallback tmp) { this->_onMouseDown = tmp; }

OnMouseEnterCallback		EventSystem::GetOnMouseEnter(void) const { return (this->_onMouseEnter); }
void		EventSystem::SetOnMouseEnter(OnMouseEnterCallback tmp) { this->_onMouseEnter = tmp; }

OnMouseExitCallback		EventSystem::GetOnMouseExit(void) const { return (this->_onMouseExit); }
void		EventSystem::SetOnMouseExit(OnMouseExitCallback tmp) { this->_onMouseExit = tmp; }

OnMouseUpCallback		EventSystem::GetOnMouseUp(void) const { return (this->_onMouseUp); }
void		EventSystem::SetOnMouseUp(OnMouseUpCallback tmp) { this->_onMouseUp = tmp; }

const Event &		EventSystem::GetCurrentEvent(void) const { return _current; }

std::ostream &	operator<<(std::ostream & o, EventSystem const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
