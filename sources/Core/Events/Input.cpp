#include "Input.hpp"
#include <vector>

using namespace LWGE;

Input::Input(void)
{
	this->_rawData = true;
	this->_keyboardMask = 0;
	this->_mouseMask = 0;
	this->_joystickMask = 0;
	this->_inputstring = "";
}

Input::Input(Input const & src)
{
	*this = src;
}

Input::~Input(void)
{
	std::cout << "Destructor of Input called" << std::endl;
}

std::vector< Event >		Input::GetEvents(void)
{
	
}


Input &	Input::operator=(Input const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_rawData = src.GetRawData();
		this->_keyboardMask = src.GetKeyboardMask();
		this->_mouseMask = src.GetMouseMask();
		this->_joystickMask = src.GetJoystickMask();
		this->_inputstring = src.GetInputstring();
		this->_mousePosition = src.GetMousePosition();
		this->_forward = src.GetForward();
		this->_right = src.GetRight();
		this->_up = src.GetUp();
	}
	return (*this);
}

bool		Input::GetRawData(void) const { return (this->_rawData); }
void		Input::SetRawData(bool tmp) { this->_rawData = tmp; }

int		Input::GetKeyboardMask(void) const { return (this->_keyboardMask); }
void		Input::SetKeyboardMask(int tmp) { this->_keyboardMask = tmp; }

short		Input::GetMouseMask(void) const { return (this->_mouseMask); }
void		Input::SetMouseMask(short tmp) { this->_mouseMask = tmp; }

short		Input::GetJoystickMask(void) const { return (this->_joystickMask); }
void		Input::SetJoystickMask(short tmp) { this->_joystickMask = tmp; }

std::string		Input::GetInputstring(void) const { return (this->_inputstring); }
void		Input::SetInputstring(std::string tmp) { this->_inputstring = tmp; }

glm::vec2		Input::GetMousePosition(void) const { return (this->_mousePosition); }
void		Input::SetMousePosition(glm::vec2 tmp) { this->_mousePosition = tmp; }

glm::vec3		Input::GetForward(void) const { return (this->_forward); }
void		Input::SetForward(glm::vec3 tmp) { this->_forward = tmp; }

glm::vec3		Input::GetRight(void) const { return (this->_right); }
void		Input::SetRight(glm::vec3 tmp) { this->_right = tmp; }

glm::vec3		Input::GetUp(void) const { return (this->_up); }
void		Input::SetUp(glm::vec3 tmp) { this->_up = tmp; }

std::ostream &	operator<<(std::ostream & o, Input const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
