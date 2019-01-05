#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "IncludeDeps.hpp"
#include "Core/Delegate.tpp"
#include "Core/KeyCode.hpp"

#include GLFW_INCLUDE
#include GLM_INCLUDE

namespace LWGC
{
	enum class ButtonAction {Press = GLFW_PRESS, Release = GLFW_RELEASE, Repeat = GLFW_REPEAT};
	enum class MouseMoveAction {Entered = GLFW_TRUE, Exited = GLFW_FALSE, Move = 2};

	class		EventSystem
	{
		private:
			GLFWwindow *			_window;

			static std::map< GLFWwindow *, EventSystem * > eventSystems;
			static EventSystem *	eventSystemInstance;

		public:
			Delegate< void(void) >								onQuit;
			Delegate< void(void) >								onFocus;
			Delegate< void(KeyCode, ButtonAction) >				onKey;
			Delegate< void(glm::vec2, ButtonAction) >			onMouseClick;
			Delegate< void(glm::vec2, MouseMoveAction) >		onMouseMove;
			glm::vec2											delta;
			glm::vec2											oldMousePosition;

			EventSystem(void);
			EventSystem(const EventSystem&) = delete;
			virtual ~EventSystem(void);

			EventSystem &	operator=(EventSystem const & src) = delete;

			void			BindWindow(GLFWwindow *window);

			void	LockCursor(void);
			void	ReleaseCursor(void);
			void	ToggleLockCursor(void);

			static EventSystem *	Get(void);
	};

	std::ostream &	operator<<(std::ostream & o, EventSystem const & r);
}
