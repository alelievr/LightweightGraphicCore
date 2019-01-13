#include "Core/Handles/Slider1DHandle.hpp"

using namespace LWGC;
using namespace Handles;

Slider1D::Slider1D(void)
{
}

Slider1D::Slider1D(Slider1D const & src)
{
	*this = src;
}

Slider1D::~Slider1D(void)
{
}

std::ostream &	operator<<(std::ostream & o, Slider1D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
