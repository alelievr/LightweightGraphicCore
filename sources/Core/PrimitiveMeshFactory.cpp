#include <memory>

#include "PrimitiveMeshFactory.hpp"

using namespace LWGE;

PrimitiveMeshFactory::PrimitiveMeshFactory(void)
{
	std::cout << "Default constructor of PrimitiveMeshFactory called" << std::endl;
}

PrimitiveMeshFactory::PrimitiveMeshFactory(PrimitiveMeshFactory const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

PrimitiveMeshFactory::~PrimitiveMeshFactory(void)
{
	std::cout << "Destructor of PrimitiveMeshFactory called" << std::endl;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateCubeMesh(void)
{
	
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateMesh(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::Cube:
			return _CreateCubeMesh();
			break ;
	}
	throw std::runtime_error("Can't find mesh generator for primitive type: " + std::to_string(static_cast< int >(type)));
}


PrimitiveMeshFactory &	PrimitiveMeshFactory::operator=(PrimitiveMeshFactory const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, PrimitiveMeshFactory const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
