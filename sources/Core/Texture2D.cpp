#include "Texture2D.hpp"

using namespace LWGC;

Texture2D::Texture2D(void)
{
	std::cout << "Default constructor of Texture2D called" << std::endl;
}

Texture2D::Texture2D(Texture2D const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Texture2D::~Texture2D(void)
{
	std::cout << "Destructor of Texture2D called" << std::endl;
}

void		Texture2D::Apply(void)
{
	
}

void		Texture2D::SetPixel(int x, int y)
{
	
}

void		Texture2D::GetPixel(int x, int y)
{
	
}

unsigned char *		Texture2D::GetRawData(void)
{
	
}

void		Texture2D::SetRawData(unsigned char * data)
{
	
}

Texture2D &	Texture2D::operator=(Texture2D const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture2D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
