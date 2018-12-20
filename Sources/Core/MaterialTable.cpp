#include "MaterialTable.hpp"


MaterialTable::MaterialTable(void)
{
	std::cout << "Default constructor of MaterialTable called" << std::endl;
}

MaterialTable::MaterialTable(MaterialTable const & src)
{
	*this = src;
	std::cout << "Copy constructor of MaterialTable called" << std::endl;
}

MaterialTable::~MaterialTable(void)
{
	std::cout << "Destructor of MaterialTable called" << std::endl;
}


MaterialTable &	MaterialTable::operator=(MaterialTable const & src)
{
	std::cout << "Assignment operator of MaterialTable called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
