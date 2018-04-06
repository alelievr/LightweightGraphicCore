#include "Event.hpp"

using namespace LWGE;

Event::Event(void)
{
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
		this->_type = src.GetType();
		this->_alt = src.GetAlt();
		this->_control = src.GetControl();
		this->_shift = src.GetShift();
		this->_function = src.GetFunction();
		this->_capsLock = src.GetCapsLock();
		this->_keyCode = src.GetKeyCode();
		this->_mouseButton = src.GetMouseButton();
		this->_joystickNumber = src.GetJoystickNumber();
		this->_delta = src.GetDelta();
	}
	return (*this);
}

EventType		Event::GetType(void) const { return (this->_type); }
void		Event::SetType(EventType tmp) { this->_type = tmp; }

bool		Event::GetAlt(void) const { return (this->_alt); }
void		Event::SetAlt(bool tmp) { this->_alt = tmp; }

bool		Event::GetControl(void) const { return (this->_control); }
void		Event::SetControl(bool tmp) { this->_control = tmp; }

bool		Event::GetShift(void) const { return (this->_shift); }
void		Event::SetShift(bool tmp) { this->_shift = tmp; }

bool		Event::GetFunction(void) const { return (this->_function); }
void		Event::SetFunction(bool tmp) { this->_function = tmp; }

bool		Event::GetCapsLock(void) const { return (this->_capsLock); }
void		Event::SetCapsLock(bool tmp) { this->_capsLock = tmp; }

int		Event::GetKeyCode(void) const { return (this->_keyCode); }
void		Event::SetKeyCode(int tmp) { this->_keyCode = tmp; }

int		Event::GetMouseButton(void) const { return (this->_mouseButton); }
void		Event::SetMouseButton(int tmp) { this->_mouseButton = tmp; }

int		Event::GetJoystickNumber(void) const { return (this->_joystickNumber); }
void		Event::SetJoystickNumber(int tmp) { this->_joystickNumber = tmp; }

glm::vec2		Event::GetDelta(void) const { return (this->_delta); }
void		Event::SetDelta(glm::vec2 tmp) { this->_delta = tmp; }

std::ostream &	operator<<(std::ostream & o, Event const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
