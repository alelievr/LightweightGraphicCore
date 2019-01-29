#include "TextureCubeArray.hpp"

TextureCubeArray::TextureCubeArray(TextureCubeArray const & src)
{
	*this = src;
}

TextureCubeArray* TextureCubeArray::Create(void)
{
	return new TextureCubeArray();
}

TextureCubeArray* TextureCubeArray::Create(const TextureCubeArray& t)
{
	return new TextureCubeArray(t);
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
