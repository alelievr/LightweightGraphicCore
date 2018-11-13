#include "Texture3D.hpp"

using namespace LWGC;

Texture3D::Texture3D(void)
{
}

Texture3D::Texture3D(Texture3D const & src)
{
	*this = src;
}

Texture3D::~Texture3D(void)
{
}


Texture3D &	Texture3D::operator=(Texture3D const & src)
{
	Texture::operator=(src);
	
	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture3D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
