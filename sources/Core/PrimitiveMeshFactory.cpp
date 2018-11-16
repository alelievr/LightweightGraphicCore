#include <memory>

#include "PrimitiveMeshFactory.hpp"

using namespace LWGC;

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

	// TODO

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateQuadMesh(void)
{
	std::shared_ptr< Mesh >		m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(4);
	
	// 0	1---0
	// 1	| \ |
	// 2    3---2
	// 3
	
	attribs[0] = Mesh::VertexAttributes{
		{ 0.5f, 0,  0.5f},	// position
		{0, 1, 0},			// normal
		{1, 0, 0},			// tangent
		{0, 0, 0},			// color
		{1, 1},				// UV
	};
	
	attribs[1] = Mesh::VertexAttributes{
		{-0.5f, 0,  0.5f},	// position
		{0, 1, 0},			// normal
		{1, 0, 0},			// tangent
		{0, 0, 0},			// color
		{0, 1},				// UV
	};
	
	attribs[2] = Mesh::VertexAttributes{
		{ 0.5f, 0, -0.5f},	// position
		{0, 1, 0},			// normal
		{1, 0, 0},			// tangent
		{0, 0, 0},			// color
		{1, 0},				// UV
	};
	
	attribs[3] = Mesh::VertexAttributes{
		{-0.5f, 0, -0.5f},	// position
		{0, 1, 0},			// normal
		{1, 0, 0},			// tangent
		{0, 0, 0},			// color
		{0, 0},				// UV
	};

	m->SetVertexAttributes(attribs);
	m->SetIndices({1, 0, 2, 2, 3, 1});

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateMesh(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::Cube:
			return _CreateCubeMesh();
		case PrimitiveType::Quad:
			return _CreateQuadMesh();
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
