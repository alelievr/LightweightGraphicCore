#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "IncludeDeps.hpp"

#include "Event.hpp"
#include GLFW_INCLUDE

namespace LWGC
{
	typedef std::function< void (void) > OnQuitCallback;
	typedef std::function< void (bool focused) > OnFocusCallback;
	typedef std::function< void (float x, float y) > OnMouseMoveCallback;
	typedef std::function< void (float x, float y, int button) > OnMouseDownCallback;
	typedef std::function< void (float x, float y, int button) > OnMouseUpCallback;
	typedef std::function< void (void) > OnMouseEnterCallback;
	typedef std::function< void (void) > OnMouseExitCallback;
	typedef std::function< void (KeyCode k) > OnKeyDownCallback;
	typedef std::function< void (KeyCode k) > OnKeyUpCallback;
	typedef std::function< void (KeyCode k) > OnKeyStayCallback;

	class		EventSystem : std::enable_shared_from_this< EventSystem >
	{
		private:
			OnQuitCallback			_onQuit;
			OnFocusCallback			_onFocus;
			
			OnMouseMoveCallback		_onMouseMove;
			OnMouseDownCallback		_onMouseDown;
			OnMouseUpCallback		_onMouseUp;

			OnMouseEnterCallback	_onMouseEnter;
			OnMouseExitCallback		_onMouseExit;

			OnKeyDownCallback		_onKeyDown;
			OnKeyUpCallback			_onKeyUp;
			OnKeyStayCallback		_onKeyStay;

			Event					_current;

			static std::map< GLFWwindow *, EventSystem * > eventSystems;
			static EventSystem *	eventSystemInstance;
			
			void DefaultMouseMoveAction(float x, float y);
			void DefaultMouseDownAction(float x, float y, int button);
			void DefaultMouseUpAction(float x, float y, int button);
			void DefaultKeyDownAction(KeyCode k);
			void DefaultKeyUpAction(KeyCode k);


		public:
			EventSystem(void);
			EventSystem(const EventSystem&) = delete;
			virtual ~EventSystem(void);

			EventSystem &	operator=(EventSystem const & src) = delete;

			void			BindWindow(GLFWwindow *window);

			OnQuitCallback	GetOnQuit(void) const;
			void	SetOnQuit(OnQuitCallback tmp);
			
			OnFocusCallback	GetOnFocus(void) const;
			void	SetOnFocus(OnFocusCallback tmp);
			
			OnMouseMoveCallback	GetOnMouseMove(void) const;
			void	SetOnMouseMove(OnMouseMoveCallback tmp);
			
			OnMouseDownCallback	GetOnMouseDown(void) const;
			void	SetOnMouseDown(OnMouseDownCallback tmp);
			
			OnMouseEnterCallback	GetOnMouseEnter(void) const;
			void	SetOnMouseEnter(OnMouseEnterCallback tmp);
			
			OnMouseExitCallback	GetOnMouseExit(void) const;
			void	SetOnMouseExit(OnMouseExitCallback tmp);
			
			OnMouseUpCallback	GetOnMouseUp(void) const;
			void	SetOnMouseUp(OnMouseUpCallback tmp);

			const Event &	GetCurrentEvent(void) const;

			static EventSystem *	Get(void);
	};

	std::ostream &	operator<<(std::ostream & o, EventSystem const & r);
}
