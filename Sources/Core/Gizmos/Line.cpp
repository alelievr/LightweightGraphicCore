#include "Line.hpp"

using namespace LWGC;
using namespace Gizmo;

Line::Line(void)
{
	std::cout << "Default constructor of Line called" << std::endl;
}

Line::Line(Line const & src)
{
	*this = src;
	std::cout << "Copy constructor of Line called" << std::endl;
}

Line::~Line(void)
{
	std::cout << "Destructor of Line called" << std::endl;
}


Line &	Line::operator=(Line const & src)
{
	std::cout << "Assignment operator of Line called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Line const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
