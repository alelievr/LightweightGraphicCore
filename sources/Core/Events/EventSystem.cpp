#include "EventSystem.hpp"

using namespace LWGE;

EventSystem::EventSystem(void)
{
	std::cout << "Default constructor of EventSystem called" << std::endl;
	this->_onQuit = nullptr;
	this->_onFocus = nullptr;
	this->_onMouseMove = nullptr;
	this->_onMouseDown = nullptr;
	this->_onMouseEnter = nullptr;
	this->_onMouseExit = nullptr;
	this->_onMouseUp = nullptr;
}

EventSystem::~EventSystem(void)
{
	std::cout << "Destructor of EventSystem called" << std::endl;
}

onQuitCallback		EventSystem::GetOnQuit(void) const { return (this->_onQuit); }
void		EventSystem::SetOnQuit(onQuitCallback tmp) { this->_onQuit = tmp; }

onFocusCallback		EventSystem::GetOnFocus(void) const { return (this->_onFocus); }
void		EventSystem::SetOnFocus(onFocusCallback tmp) { this->_onFocus = tmp; }

onMouseMoveCallback		EventSystem::GetOnMouseMove(void) const { return (this->_onMouseMove); }
void		EventSystem::SetOnMouseMove(onMouseMoveCallback tmp) { this->_onMouseMove = tmp; }

onMouseDownCallback		EventSystem::GetOnMouseDown(void) const { return (this->_onMouseDown); }
void		EventSystem::SetOnMouseDown(onMouseDownCallback tmp) { this->_onMouseDown = tmp; }

onMouseEnterCallback		EventSystem::GetOnMouseEnter(void) const { return (this->_onMouseEnter); }
void		EventSystem::SetOnMouseEnter(onMouseEnterCallback tmp) { this->_onMouseEnter = tmp; }

onMouseExitCallback		EventSystem::GetOnMouseExit(void) const { return (this->_onMouseExit); }
void		EventSystem::SetOnMouseExit(onMouseExitCallback tmp) { this->_onMouseExit = tmp; }

onMouseUpCallback		EventSystem::GetOnMouseUp(void) const { return (this->_onMouseUp); }
void		EventSystem::SetOnMouseUp(onMouseUpCallback tmp) { this->_onMouseUp = tmp; }

Event			EventSystem::GetCurrentEvent(void) const { return _current; }

std::ostream &	operator<<(std::ostream & o, EventSystem const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
