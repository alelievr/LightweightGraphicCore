#include "Object.hpp"

using namespace LWGC;

Object::Object(void)
{
	this->_name = "Object";
	this->_flags = 0;
}

Object::Object(Object const & src)
{
	*this = src;
}

Object::~Object(void)
{
}

Object &	Object::operator=(Object const & src)
{
	if (this != &src) {
		this->_name = src.GetName();
		this->_flags = src.GetFlags();
	}
	return (*this);
}

std::string	Object::GetName(void) const { return (this->_name); }
void		Object::SetName(const std::string & tmp) { this->_name = tmp; }

int			Object::GetFlags(void) const { return (this->_flags); }
void		Object::SetFlags(int tmp) { this->_flags = tmp; }

std::ostream &	operator<<(std::ostream & o, Object const & r)
{
	o << r.GetName();
	return (o);
}
