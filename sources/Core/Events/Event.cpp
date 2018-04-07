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
		this->type = src.GetType();
		this->alt = src.GetAlt();
		this->control = src.GetControl();
		this->shift = src.GetShift();
		this->function = src.GetFunction();
		this->capsLock = src.GetCapsLock();
		this->keyCode = src.GetKeyCode();
		this->mouseButton = src.GetMouseButton();
		this->joystickNumber = src.GetJoystickNumber();
		this->delta = src.GetDelta();
	}
	return (*this);
}

EventType		Event::GetType(void) const { return (this->type); }
void		Event::SetType(EventType tmp) { this->type = tmp; }

bool		Event::GetAlt(void) const { return (this->alt); }
void		Event::SetAlt(bool tmp) { this->alt = tmp; }

bool		Event::GetControl(void) const { return (this->control); }
void		Event::SetControl(bool tmp) { this->control = tmp; }

bool		Event::GetShift(void) const { return (this->shift); }
void		Event::SetShift(bool tmp) { this->shift = tmp; }

bool		Event::GetFunction(void) const { return (this->function); }
void		Event::SetFunction(bool tmp) { this->function = tmp; }

bool		Event::GetCapsLock(void) const { return (this->capsLock); }
void		Event::SetCapsLock(bool tmp) { this->capsLock = tmp; }

KeyCode		Event::GetKeyCode(void) const { return (this->keyCode); }
void		Event::SetKeyCode(KeyCode tmp) { this->keyCode = tmp; }

int		Event::GetMouseButton(void) const { return (this->mouseButton); }
void		Event::SetMouseButton(int tmp) { this->mouseButton = tmp; }

int		Event::GetJoystickNumber(void) const { return (this->joystickNumber); }
void		Event::SetJoystickNumber(int tmp) { this->joystickNumber = tmp; }

glm::vec2		Event::GetDelta(void) const { return (this->delta); }
void		Event::SetDelta(glm::vec2 tmp) { this->delta = tmp; }

glm::vec2	Event::GetMousePosition(void) const { return (this->mousePosition); }
void	Event::SetMousePosition(glm::vec2 tmp) { this->mousePosition = tmp; }
void	Event::SetMousePosition(float x, float y) { this->mousePosition.x = x; this->mousePosition.y = y; }

std::ostream &	operator<<(std::ostream & o, Event const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
