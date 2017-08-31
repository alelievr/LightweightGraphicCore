#include "EventSystem.hpp"


EventSystem::EventSystem(void)
{
	std::cout << "Default constructor of EventSystem called" << std::endl;
	this->_onQuit = ;
	this->_onFocus = ;
	this->_onMove = ;
	this->_onMouseDown = ;
	this->_onMouseDrag = ;
	this->_onMouseEnter = ;
	this->_onMouseExit = ;
	this->_onMouseOver = ;
	this->_onMouseUp = ;
}

EventSystem::EventSystem(EventSystem const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

EventSystem::~EventSystem(void)
{
	std::cout << "Destructor of EventSystem called" << std::endl;
}


EventSystem &	EventSystem::operator=(EventSystem const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_onQuit = src.getOnQuit();
		this->_onFocus = src.getOnFocus();
		this->_onMove = src.getOnMove();
		this->_onMouseDown = src.getOnMouseDown();
		this->_onMouseDrag = src.getOnMouseDrag();
		this->_onMouseEnter = src.getOnMouseEnter();
		this->_onMouseExit = src.getOnMouseExit();
		this->_onMouseOver = src.getOnMouseOver();
		this->_onMouseUp = src.getOnMouseUp();
	}
	return (*this);
}

onQuitCallback		EventSystem::getOnQuit(void) const { return (this->_onQuit); }
void		EventSystem::setOnQuit(onQuitCallback tmp) { this->_onQuit = tmp; }

onFocusCallback		EventSystem::getOnFocus(void) const { return (this->_onFocus); }
void		EventSystem::setOnFocus(onFocusCallback tmp) { this->_onFocus = tmp; }

onMoveCallback		EventSystem::getOnMove(void) const { return (this->_onMove); }
void		EventSystem::setOnMove(onMoveCallback tmp) { this->_onMove = tmp; }

onMouseDownCallback		EventSystem::getOnMouseDown(void) const { return (this->_onMouseDown); }
void		EventSystem::setOnMouseDown(onMouseDownCallback tmp) { this->_onMouseDown = tmp; }

onMouseDragCallback		EventSystem::getOnMouseDrag(void) const { return (this->_onMouseDrag); }
void		EventSystem::setOnMouseDrag(onMouseDragCallback tmp) { this->_onMouseDrag = tmp; }

onMouseEnterCallback		EventSystem::getOnMouseEnter(void) const { return (this->_onMouseEnter); }
void		EventSystem::setOnMouseEnter(onMouseEnterCallback tmp) { this->_onMouseEnter = tmp; }

onMouseExitCallback		EventSystem::getOnMouseExit(void) const { return (this->_onMouseExit); }
void		EventSystem::setOnMouseExit(onMouseExitCallback tmp) { this->_onMouseExit = tmp; }

onMouseOverCallback		EventSystem::getOnMouseOver(void) const { return (this->_onMouseOver); }
void		EventSystem::setOnMouseOver(onMouseOverCallback tmp) { this->_onMouseOver = tmp; }

onMouseUpCallback		EventSystem::getOnMouseUp(void) const { return (this->_onMouseUp); }
void		EventSystem::setOnMouseUp(onMouseUpCallback tmp) { this->_onMouseUp = tmp; }

std::ostream &	operator<<(std::ostream & o, EventSystem const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
