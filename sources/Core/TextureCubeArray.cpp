#include "TextureCubeArray.hpp"


TextureCubeArray::TextureCubeArray(void)
{
	std::cout << "Default constructor of TextureCubeArray called" << std::endl;
}

TextureCubeArray::TextureCubeArray(TextureCubeArray const & src)
{
	*this = src;
	std::cout << "Copy constructor of TextureCubeArray called" << std::endl;
}

TextureCubeArray::~TextureCubeArray(void)
{
	std::cout << "Destructor of TextureCubeArray called" << std::endl;
}


TextureCubeArray &	TextureCubeArray::operator=(TextureCubeArray const & src)
{
	std::cout << "Assignment operator of TextureCubeArray called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, TextureCubeArray const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
