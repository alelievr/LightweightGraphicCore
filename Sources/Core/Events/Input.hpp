#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IncludeDeps.hpp"

#include GLM_INCLUDE
#include "Event.hpp"

namespace LWGC
{
	class		Input
	{
		private:
			static bool		_rawData;
			static int		_keyboardMask;
			static short	_mouseMask;
			static short	_joystickMask;
			static std::string	_inputstring;
			static glm::vec2	_mousePosition;
			static glm::vec3	_forward;
			static glm::vec3	_right;
			static glm::vec3	_up;


		public:
			Input(void);
			Input(const Input&);
			virtual ~Input(void);

			Input &	operator=(Input const & src);

			static std::vector< Event >	GetEvents(void);

			bool	GetRawData(void) const;
			void	SetRawData(bool tmp);
			
			int	GetKeyboardMask(void) const;
			void	SetKeyboardMask(int tmp);
			
			short	GetMouseMask(void) const;
			void	SetMouseMask(short tmp);
			
			short	GetJoystickMask(void) const;
			void	SetJoystickMask(short tmp);
			
			std::string	GetInputstring(void) const;
			void	SetInputstring(std::string tmp);
			
			glm::vec2	GetMousePosition(void) const;
			void	SetMousePosition(glm::vec2 tmp);
			
			glm::vec3	GetForward(void) const;
			void	SetForward(glm::vec3 tmp);
			
			glm::vec3	GetRight(void) const;
			void	SetRight(glm::vec3 tmp);
			
			glm::vec3	GetUp(void) const;
			void	SetUp(glm::vec3 tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Input const & r);
}
