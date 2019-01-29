#include "TextureTable.hpp"

using namespace LWGC;

TextureTable::TextureTable(void)
{
}

TextureTable::~TextureTable(void)
{
}

std::ostream &	operator<<(std::ostream & o, TextureTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}