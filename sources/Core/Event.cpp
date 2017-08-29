#include "Event.hpp"


Event::Event(void)
{
	std::cout << "Default constructor of Event called" << std::endl;
	this->_type = ;
	this->_alt = true;
	this->_control = true;
	this->_shift = true;
	this->_function = true;
	this->_capsLock = true;
	this->_keyCode = 0;
	this->_mouseButton = 0;
	this->_joystickNumber = 0;
	this->_delta = ;
}

Event::Event(Event const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Event::~Event(void)
{
	std::cout << "Destructor of Event called" << std::endl;
}


Event &	Event::operator=(Event const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_type = src.getType();
		this->_alt = src.getAlt();
		this->_control = src.getControl();
		this->_shift = src.getShift();
		this->_function = src.getFunction();
		this->_capsLock = src.getCapsLock();
		this->_keyCode = src.getKeyCode();
		this->_mouseButton = src.getMouseButton();
		this->_joystickNumber = src.getJoystickNumber();
		this->_delta = src.getDelta();
	}
	return (*this);
}

EventType		Event::getType(void) const { return (this->_type); }
void		Event::setType(EventType tmp) { this->_type = tmp; }

bool		Event::getAlt(void) const { return (this->_alt); }
void		Event::setAlt(bool tmp) { this->_alt = tmp; }

bool		Event::getControl(void) const { return (this->_control); }
void		Event::setControl(bool tmp) { this->_control = tmp; }

bool		Event::getShift(void) const { return (this->_shift); }
void		Event::setShift(bool tmp) { this->_shift = tmp; }

bool		Event::getFunction(void) const { return (this->_function); }
void		Event::setFunction(bool tmp) { this->_function = tmp; }

bool		Event::getCapsLock(void) const { return (this->_capsLock); }
void		Event::setCapsLock(bool tmp) { this->_capsLock = tmp; }

int		Event::getKeyCode(void) const { return (this->_keyCode); }
void		Event::setKeyCode(int tmp) { this->_keyCode = tmp; }

int		Event::getMouseButton(void) const { return (this->_mouseButton); }
void		Event::setMouseButton(int tmp) { this->_mouseButton = tmp; }

int		Event::getJoystickNumber(void) const { return (this->_joystickNumber); }
void		Event::setJoystickNumber(int tmp) { this->_joystickNumber = tmp; }

glm::vec2		Event::getDelta(void) const { return (this->_delta); }
void		Event::setDelta(glm::vec2 tmp) { this->_delta = tmp; }

std::ostream &	operator<<(std::ostream & o, Event const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
