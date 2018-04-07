#pragma once

#include <iostream>
#include <string>

#include "glm/glm.hpp"

#include "EventType.hpp"
#include "../KeyCode.hpp"

namespace LWGE
{
	class		Event
	{
		private:
			EventType	_type;
			bool		_alt;
			bool		_control;
			bool		_shift;
			bool		_function;
			bool		_capsLock;
			KeyCode		_keyCode;
			int			_mouseButton;
			int			_joystickNumber;
			glm::vec2	_delta;


		public:
			Event(void);
			Event(const Event&);
			virtual ~Event(void);

			Event &	operator=(Event const & src);

			EventType	GetType(void) const;
			void	SetType(EventType tmp);
			
			bool	GetAlt(void) const;
			void	SetAlt(bool tmp);
			
			bool	GetControl(void) const;
			void	SetControl(bool tmp);
			
			bool	GetShift(void) const;
			void	SetShift(bool tmp);
			
			bool	GetFunction(void) const;
			void	SetFunction(bool tmp);
			
			bool	GetCapsLock(void) const;
			void	SetCapsLock(bool tmp);
			
			KeyCode	GetKeyCode(void) const;
			void	SetKeyCode(KeyCode tmp);
			
			int	GetMouseButton(void) const;
			void	SetMouseButton(int tmp);
			
			int	GetJoystickNumber(void) const;
			void	SetJoystickNumber(int tmp);
			
			glm::vec2	GetDelta(void) const;
			void	SetDelta(glm::vec2 tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Event const & r);
}
