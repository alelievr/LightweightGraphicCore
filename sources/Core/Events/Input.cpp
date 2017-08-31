#include "Input.hpp"


Input::Input(void)
{
	std::cout << "Default constructor of Input called" << std::endl;
	this->_rawData = true;
	this->_keyboardMask = 0;
	this->_mouseMask = ;
	this->_joystickMask = ;
	this->_inputstring = "";
	this->_mousePosition = ;
	this->_forward = ;
	this->_right = ;
	this->_up = ;
}

Input::Input(Input const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Input::~Input(void)
{
	std::cout << "Destructor of Input called" << std::endl;
}

static std::vector< Event >		Input::GetEvents(void)
{
	
}


Input &	Input::operator=(Input const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_rawData = src.getRawData();
		this->_keyboardMask = src.getKeyboardMask();
		this->_mouseMask = src.getMouseMask();
		this->_joystickMask = src.getJoystickMask();
		this->_inputstring = src.getInputstring();
		this->_mousePosition = src.getMousePosition();
		this->_forward = src.getForward();
		this->_right = src.getRight();
		this->_up = src.getUp();
	}
	return (*this);
}

bool		Input::getRawData(void) const { return (this->_rawData); }
void		Input::setRawData(bool tmp) { this->_rawData = tmp; }

int		Input::getKeyboardMask(void) const { return (this->_keyboardMask); }
void		Input::setKeyboardMask(int tmp) { this->_keyboardMask = tmp; }

short		Input::getMouseMask(void) const { return (this->_mouseMask); }
void		Input::setMouseMask(short tmp) { this->_mouseMask = tmp; }

short		Input::getJoystickMask(void) const { return (this->_joystickMask); }
void		Input::setJoystickMask(short tmp) { this->_joystickMask = tmp; }

std::string		Input::getInputstring(void) const { return (this->_inputstring); }
void		Input::setInputstring(std::string tmp) { this->_inputstring = tmp; }

glm::vec2		Input::getMousePosition(void) const { return (this->_mousePosition); }
void		Input::setMousePosition(glm::vec2 tmp) { this->_mousePosition = tmp; }

glm::vec3		Input::getForward(void) const { return (this->_forward); }
void		Input::setForward(glm::vec3 tmp) { this->_forward = tmp; }

glm::vec3		Input::getRight(void) const { return (this->_right); }
void		Input::setRight(glm::vec3 tmp) { this->_right = tmp; }

glm::vec3		Input::getUp(void) const { return (this->_up); }
void		Input::setUp(glm::vec3 tmp) { this->_up = tmp; }

std::ostream &	operator<<(std::ostream & o, Input const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
