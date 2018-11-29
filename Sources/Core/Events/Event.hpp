#pragma once
#include "IncludeDeps.hpp"

#include <iostream>
#include <string>

#include GLM_INCLUDE

#include "EventType.hpp"
#include "../KeyCode.hpp"

namespace LWGC
{
	struct		Event
	{
		public:
			EventType	type;
			bool		alt;
			bool		control;
			bool		shift;
			bool		function;
			bool		capsLock;
			KeyCode		keyCode;
			int			mouseButton;
			int			joystickNumber;
			glm::vec2	delta;
			glm::vec2	mousePosition;

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

			glm::vec2	GetMousePosition(void) const;
			void	SetMousePosition(glm::vec2 tmp);
			void	SetMousePosition(float x, float y);
	};

	std::ostream &	operator<<(std::ostream & o, Event const & r);
}
