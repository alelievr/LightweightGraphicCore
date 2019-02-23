#include "Texture2DAtlas.hpp"


Texture2DAtlas::Texture2DAtlas(void)
{
	std::cout << "Default constructor of Texture2DAtlas called" << std::endl;
}

Texture2DAtlas::Texture2DAtlas(Texture2DAtlas const & src)
{
	*this = src;
	std::cout << "Copy constructor of Texture2DAtlas called" << std::endl;
}

Texture2DAtlas::~Texture2DAtlas(void)
{
	std::cout << "Destructor of Texture2DAtlas called" << std::endl;
}


Texture2DAtlas &	Texture2DAtlas::operator=(Texture2DAtlas const & src)
{
	std::cout << "Assignment operator of Texture2DAtlas called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
