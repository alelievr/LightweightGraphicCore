#include "Texture2DArray.hpp"


Texture2DArray::Texture2DArray(void)
{
	std::cout << "Default constructor of Texture2DArray called" << std::endl;
}

Texture2DArray::Texture2DArray(Texture2DArray const & src)
{
	*this = src;
	std::cout << "Copy constructor of Texture2DArray called" << std::endl;
}

Texture2DArray::~Texture2DArray(void)
{
	std::cout << "Destructor of Texture2DArray called" << std::endl;
}


Texture2DArray &	Texture2DArray::operator=(Texture2DArray const & src)
{
	std::cout << "Assignment operator of Texture2DArray called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture2DArray const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
