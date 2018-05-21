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
	std::shared_ptr< Mesh >		m = std::make_shared< Mesh >();

	m->AddVertex(-1, -1, -1);
	m->AddVertex( 1, -1, -1);
	m->AddVertex( 1, -1,  1);
	m->AddVertex(-1, -1,  1);

	m->AddVertex(-1,  1, -1);
	m->AddVertex( 1,  1, -1);
	m->AddVertex( 1,  1,  1);
	m->AddVertex(-1,  1,  1);

	//up
	m->AddTriangle(2 + 4, 1 + 4, 0 + 4);
	m->AddTriangle(0 + 4, 2 + 4, 3 + 4);

	//down
	m->AddTriangle(0, 1, 2);
	m->AddTriangle(0, 2, 3);

	m->UploadDatas();

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateMesh(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::Cube:
			return _CreateCubeMesh();
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
