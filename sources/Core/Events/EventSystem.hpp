#pragma once

#include <iostream>
#include <string>
#include <functional>

#include "Event.hpp"

namespace LWGE
{
	typedef std::function< void (void) > onQuitCallback;
	typedef std::function< void (bool focused) > onFocusCallback;
	typedef std::function< void (float x, float y) > onMouseMoveCallback;
	typedef std::function< void (float x, float y, int button) > onMouseDownCallback;
	typedef std::function< void (float x, float y, int button) > onMouseUpCallback;
	typedef std::function< void (void) > onMouseEnterCallback;
	typedef std::function< void (void) > onMouseExitCallback;

	class		EventSystem
	{
		private:
			onQuitCallback			_onQuit;
			onFocusCallback			_onFocus;
			onMouseMoveCallback		_onMouseMove;
			onMouseDownCallback		_onMouseDown;
			onMouseEnterCallback	_onMouseEnter;
			onMouseExitCallback		_onMouseExit;
			onMouseUpCallback		_onMouseUp;

			Event						_current;


		public:
			EventSystem(void);
			EventSystem(const EventSystem&) = delete;
			virtual ~EventSystem(void);

			EventSystem &	operator=(EventSystem const & src) = delete;

			onQuitCallback	GetOnQuit(void) const;
			void	SetOnQuit(onQuitCallback tmp);
			
			onFocusCallback	GetOnFocus(void) const;
			void	SetOnFocus(onFocusCallback tmp);
			
			onMouseMoveCallback	GetOnMouseMove(void) const;
			void	SetOnMouseMove(onMouseMoveCallback tmp);
			
			onMouseDownCallback	GetOnMouseDown(void) const;
			void	SetOnMouseDown(onMouseDownCallback tmp);
			
			onMouseEnterCallback	GetOnMouseEnter(void) const;
			void	SetOnMouseEnter(onMouseEnterCallback tmp);
			
			onMouseExitCallback	GetOnMouseExit(void) const;
			void	SetOnMouseExit(onMouseExitCallback tmp);
			
			onMouseUpCallback	GetOnMouseUp(void) const;
			void	SetOnMouseUp(onMouseUpCallback tmp);

			Event	GetCurrentEvent(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, EventSystem const & r);
}
