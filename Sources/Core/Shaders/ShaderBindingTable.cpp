#include "ShaderBindingTable.hpp"

using namespace LWGC;

ShaderBindingTable::ShaderBindingTable(void)
{
	std::cout << "Default constructor of ShaderBindingTable called" << std::endl;
}

ShaderBindingTable::~ShaderBindingTable(void)
{
	std::cout << "Destructor of ShaderBindingTable called" << std::endl;
}

std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
