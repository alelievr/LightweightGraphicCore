#ifndef INPUT_HPP
# define INPUT_HPP
# include <iostream>
# include <string>

class		Input
{
	private:
		static bool	_rawData;
		static int	_keyboardMask;
		static short	_mouseMask;
		static short	_joystickMask;
		static std::string	_inputstring;
		static glm::vec2	_mousePosition;
		static glm::vec3	_forward;
		static glm::vec3	_right;
		static glm::vec3	_up;


	public:
		Input();
		Input(const Input&);
		virtual ~Input(void);

		Input &	operator=(Input const & src);

		static std::vector< Event >	GetEvents(void);

		bool	getRawData(void) const;
		void	setRawData(bool tmp);
		
		int	getKeyboardMask(void) const;
		void	setKeyboardMask(int tmp);
		
		short	getMouseMask(void) const;
		void	setMouseMask(short tmp);
		
		short	getJoystickMask(void) const;
		void	setJoystickMask(short tmp);
		
		std::string	getInputstring(void) const;
		void	setInputstring(std::string tmp);
		
		glm::vec2	getMousePosition(void) const;
		void	setMousePosition(glm::vec2 tmp);
		
		glm::vec3	getForward(void) const;
		void	setForward(glm::vec3 tmp);
		
		glm::vec3	getRight(void) const;
		void	setRight(glm::vec3 tmp);
		
		glm::vec3	getUp(void) const;
		void	setUp(glm::vec3 tmp);
};

std::ostream &	operator<<(std::ostream & o, Input const & r);

#endif
