#include "TextureCube.hpp"

using namespace LWGC;

TextureCube::TextureCube(void)
{
}

TextureCube::TextureCube(TextureCube const & src)
{
	*this = src;
}

TextureCube::~TextureCube(void)
{
}


TextureCube &	TextureCube::operator=(TextureCube const & src)
{
	Texture::operator=(src);

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, TextureCube const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
