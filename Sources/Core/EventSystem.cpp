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
			(void)mods;
			auto & self = eventSystems[window];
			double posX;
			double posY;
			glfwGetCursorPos(window, &posX, &posY);
			const glm::vec2 & mousePosition = glm::vec2(posX, posY);

			self->onMouseClick.Invoke(mousePosition, button, static_cast< ButtonAction >(action));
		}
	);
	glfwSetCursorPosCallback(window, [](GLFWwindow * window, double posX, double posY)
		{
			auto & self = eventSystems[window];
			glm::vec2 virtualMousePos = glm::vec2(posX, posY);
			self->UpdateMousePosition();

			self->delta = virtualMousePos - self->oldMousePosition;
			self->oldMousePosition = virtualMousePos;
			self->onMouseMove.Invoke(virtualMousePos, MouseMoveAction::Move);
		}
	);

	UpdateMousePosition();

	delta = _mousePosition - oldMousePosition;
	oldMousePosition = _mousePosition;
	onMouseMove.Invoke(_mousePosition, MouseMoveAction::Move);

	Application::update.AddListener([&](){
		}
	);

	Application::lateUpdate.AddListener([&](){
			this->delta = glm::vec2(0, 0);
		}
	);
}

void				EventSystem::UpdateMousePosition(void)
{
	double posX;
	double posY;
	glfwGetCursorPos(_window, &posX, &posY);

	// If the cursor is lock, the only possible position is the middle of the screen
	if (IsCursorLocked())
	{
		auto extent = VulkanRenderPipeline::Get()->GetSwapChain()->GetExtent();
		_mousePosition = glm::vec2(extent.width, extent.height) / 2.0f;
	}
	else
		_mousePosition = glm::vec2(posX, posY);
}

void				EventSystem::LockCursor(void)
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	UpdateMousePosition();
}

void				EventSystem::ReleaseCursor(void)
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	UpdateMousePosition();
}

void				EventSystem::ToggleLockCursor(void)
{
	if (IsCursorLocked())
		ReleaseCursor();
	else
		LockCursor();
}

bool				EventSystem::IsCursorLocked(void)
{
	return glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

const glm::vec2		EventSystem::GetCursorPosition(void) const { return _mousePosition; }

EventSystem *		EventSystem::Get(void) { return eventSystemInstance; }

std::ostream &	operator<<(std::ostream & o, EventSystem const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
