#include "Application.hpp"


Application::Application(void)
{
	std::cout << "Default constructor of Application called" << std::endl;
	this->_renderPipeline = NULL;
	this->_eventSystem = ;
}

Application::Application(Application const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Application::~Application(void)
{
	std::cout << "Destructor of Application called" << std::endl;
}


Application &	Application::operator=(Application const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_renderPipeline = src.getRenderPipeline();
		this->_eventSystem = src.getEventSystem();
	}
	return (*this);
}

IRenderPipeline *		Application::getRenderPipeline(void) const { return (this->_renderPipeline); }
void		Application::setRenderPipeline(IRenderPipeline * tmp) { this->_renderPipeline = tmp; }

EventSystem		Application::getEventSystem(void) const { return (this->_eventSystem); }
void		Application::setEventSystem(EventSystem tmp) { this->_eventSystem = tmp; }

std::ostream &	operator<<(std::ostream & o, Application const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
