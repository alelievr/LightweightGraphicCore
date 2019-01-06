#include "TextureCube.hpp"

using namespace LWGC;

TextureCube::TextureCube(TextureCube const & src)
{
	*this = src;
}

TextureCube*			TextureCube::Create(void)
{
	return new TextureCube();
}

TextureCube*			TextureCube::Create(const TextureCube & t)
{
	return new TextureCube(t);
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
