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
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(24);

	glm::vec3 p0 = glm::vec3(-.5f, -.5f,  .5f);
	glm::vec3 p1 = glm::vec3( .5f, -.5f,  .5f);
	glm::vec3 p2 = glm::vec3( .5f, -.5f, -.5f);
	glm::vec3 p3 = glm::vec3(-.5f, -.5f, -.5f);

	glm::vec3 p4 = glm::vec3(-.5f,  .5f,  .5f);
	glm::vec3 p5 = glm::vec3( .5f,  .5f,  .5f);
	glm::vec3 p6 = glm::vec3( .5f,  .5f, -.5f);
	glm::vec3 p7 = glm::vec3(-.5f,  .5f, -.5f);

	// Bottom
	Mesh::VertexAttributes::QuadVertexAttrib(p0, p1, p2, p3, attribs.data() + 0);
	// Left
	Mesh::VertexAttributes::QuadVertexAttrib(p7, p4, p0, p3, attribs.data() + 4);
	// Front
	Mesh::VertexAttributes::QuadVertexAttrib(p4, p5, p1, p0, attribs.data() + 8);
	// Back
	Mesh::VertexAttributes::QuadVertexAttrib(p6, p7, p3, p2, attribs.data() + 12);
	// Right
	Mesh::VertexAttributes::QuadVertexAttrib(p5, p6, p2, p1, attribs.data() + 16);
	// Top
	Mesh::VertexAttributes::QuadVertexAttrib(p7, p6, p5, p4, attribs.data() + 20);

	m->SetVertexAttributes(attribs);
	m->SetIndices({
		// Bottom
		3, 1, 0,
		3, 2, 1,

		// Left
		3 + 4 * 1, 1 + 4 * 1, 0 + 4 * 1,
		3 + 4 * 1, 2 + 4 * 1, 1 + 4 * 1,

		// Front
		3 + 4 * 2, 1 + 4 * 2, 0 + 4 * 2,
		3 + 4 * 2, 2 + 4 * 2, 1 + 4 * 2,

		// Back
		3 + 4 * 3, 1 + 4 * 3, 0 + 4 * 3,
		3 + 4 * 3, 2 + 4 * 3, 1 + 4 * 3,

		// Right
		3 + 4 * 4, 1 + 4 * 4, 0 + 4 * 4,
		3 + 4 * 4, 2 + 4 * 4, 1 + 4 * 4,

		// Top
		3 + 4 * 5, 1 + 4 * 5, 0 + 4 * 5,
		3 + 4 * 5, 2 + 4 * 5, 1 + 4 * 5,
	});

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateQuadMesh(void)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(4);

	Mesh::VertexAttributes::QuadVertexAttrib(1, glm::vec3(0, 1, 0), attribs.data());

	m->SetVertexAttributes(attribs);
	m->SetIndices({0, 1, 2, 2, 3, 0});

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
