#include "Object.hpp"


Object::Object(void)
{
	std::cout << "Default constructor of Object called" << std::endl;
	this->_trasform = ;
	this->_name = "";
	this->_flags = 0;
}

Object::Object(Object const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Object::~Object(void)
{
	std::cout << "Destructor of Object called" << std::endl;
}


Object &	Object::operator=(Object const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_trasform = src.getTrasform();
		this->_name = src.getName();
		this->_flags = src.getFlags();
	}
	return (*this);
}

Trasform		Object::getTrasform(void) const { return (this->_trasform); }
void		Object::setTrasform(Trasform tmp) { this->_trasform = tmp; }

std::string		Object::getName(void) const { return (this->_name); }
void		Object::setName(std::string tmp) { this->_name = tmp; }

int		Object::getFlags(void) const { return (this->_flags); }
void		Object::setFlags(int tmp) { this->_flags = tmp; }

std::ostream &	operator<<(std::ostream & o, Object const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
