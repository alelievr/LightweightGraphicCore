#include <memory>

#include "PrimitiveMeshFactory.hpp"

#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

void						PrimitiveMeshFactory::_ComputeFrustumPoints(float fovY, float aspect, float nearPlane, float farPlane, std::vector< glm::vec3 > & p)
{
	// near plane
	p[6] = glm::vec3( .5f,  .5f, nearPlane); //  3------0
	p[2] = glm::vec3( .5f, -.5f, nearPlane); //  |      |
	p[3] = glm::vec3(-.5f, -.5f, nearPlane); //  |      |
	p[7] = glm::vec3(-.5f,  .5f, nearPlane); //  2------1

	// far plane
	p[5] = glm::vec3( .5f,  .5f, farPlane);
	p[1] = glm::vec3( .5f, -.5f, farPlane);
	p[0] = glm::vec3(-.5f, -.5f, farPlane);
	p[4] = glm::vec3(-.5f,  .5f, farPlane);

	float fovX = 2.0f * atan(tan(fovY / 2.0f) * aspect);

	// sine law
	p[6].x = sin(fovX) * (nearPlane / sin(Math::DegToRad * 90.0f - fovX));
	p[6].y = sin(fovY) * (nearPlane / sin(Math::DegToRad * 90.0f - fovY));
	p[2].x =  p[6].x; p[2].y = -p[6].y;
	p[3].x = -p[6].x; p[3].y = -p[6].y;
	p[7].x = -p[6].x; p[7].y =  p[6].y;

	p[5].x = sin(fovX) * (farPlane / sin(Math::DegToRad * 90.0f - fovX));
	p[5].y = sin(fovY) * (farPlane / sin(Math::DegToRad * 90.0f - fovY));
	p[1].x =  p[5].x; p[1].y = -p[5].y;
	p[0].x = -p[5].x; p[0].y = -p[5].y;
	p[4].x = -p[5].x; p[4].y =  p[5].y;
}

void						PrimitiveMeshFactory::_ComputeCubePoints(float size, std::vector< glm::vec3 > & p)
{
	p[0] = glm::vec3(-.5f, -.5f,  .5f) * size;
	p[1] = glm::vec3( .5f, -.5f,  .5f) * size;
	p[2] = glm::vec3( .5f, -.5f, -.5f) * size;
	p[3] = glm::vec3(-.5f, -.5f, -.5f) * size;

	p[4] = glm::vec3(-.5f,  .5f,  .5f) * size;
	p[5] = glm::vec3( .5f,  .5f,  .5f) * size;
	p[6] = glm::vec3( .5f,  .5f, -.5f) * size;
	p[7] = glm::vec3(-.5f,  .5f, -.5f) * size;
}

 void						PrimitiveMeshFactory::_ComputeCirclePoints(float radius, int vertexCount, std::vector< glm::vec3 > & points)
{
	float		c = 0;

	for (int i = 0; i < vertexCount; i++)
	{
		points[i] = glm::vec3(sin(c), 0, cos(c)) * radius;
		c += M_PI * 2 / vertexCount;
	}
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateCubeMesh(const std::vector< glm::vec3 > p)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(24);

	// Bottom
	Mesh::VertexAttributes::QuadVertexAttrib(p[0], p[1], p[2], p[3], attribs.data() + 0);
	// Left
	Mesh::VertexAttributes::QuadVertexAttrib(p[7], p[4], p[0], p[3], attribs.data() + 4);
	// Front
	Mesh::VertexAttributes::QuadVertexAttrib(p[4], p[5], p[1], p[0], attribs.data() + 8);
	// Back
	Mesh::VertexAttributes::QuadVertexAttrib(p[6], p[7], p[3], p[2], attribs.data() + 12);
	// Right
	Mesh::VertexAttributes::QuadVertexAttrib(p[5], p[6], p[2], p[1], attribs.data() + 16);
	// Top
	Mesh::VertexAttributes::QuadVertexAttrib(p[7], p[6], p[5], p[4], attribs.data() + 20);

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

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateWireframeCubeMesh(const std::vector< glm::vec3 > p)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(24);

	// top quad
	Mesh::VertexAttributes::EdgeVertexAttrib(p[4], p[5], attribs.data() + 0);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[5], p[6], attribs.data() + 2);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[6], p[7], attribs.data() + 4);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[7], p[4], attribs.data() + 6);

	// bottom quad
	Mesh::VertexAttributes::EdgeVertexAttrib(p[0], p[1], attribs.data() + 8);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[1], p[2], attribs.data() + 10);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[2], p[3], attribs.data() + 12);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[3], p[0], attribs.data() + 14);

	// middle edges
	Mesh::VertexAttributes::EdgeVertexAttrib(p[0], p[4], attribs.data() + 16);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[1], p[5], attribs.data() + 18);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[2], p[6], attribs.data() + 20);
	Mesh::VertexAttributes::EdgeVertexAttrib(p[3], p[7], attribs.data() + 22);

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateCubeMesh(void)
{
	std::vector< glm::vec3 > cubePoints(8);

	_ComputeCubePoints(1, cubePoints);

	return _CreateCubeMesh(cubePoints);
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateWireframeCubeMesh(void)
{
	std::vector< glm::vec3 >	cubePoints;

	_ComputeCubePoints(1, cubePoints);
	return _CreateWireframeCubeMesh(cubePoints);
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

std::shared_ptr< Mesh >		PrimitiveMeshFactory::_CreateWireframeQuadMesh(void)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(8);

	//  3------0
	//  |      |
	//  |      |
	//  2------1

	glm::vec3 p0 = glm::vec3( 0.5,  0.5, 0);
	glm::vec3 p1 = glm::vec3( 0.5, -0.5, 0);
	glm::vec3 p2 = glm::vec3(-0.5, -0.5, 0);
	glm::vec3 p3 = glm::vec3(-0.5,  0.5, 0);

	Mesh::VertexAttributes::EdgeVertexAttrib(p0, p1, attribs.data() + 0);
	Mesh::VertexAttributes::EdgeVertexAttrib(p1, p2, attribs.data() + 1);
	Mesh::VertexAttributes::EdgeVertexAttrib(p2, p3, attribs.data() + 2);
	Mesh::VertexAttributes::EdgeVertexAttrib(p3, p0, attribs.data() + 3);

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateFrustum(float fovY, float aspect, float nearPlane, float farPlane)
{
	std::vector< glm::vec3 >	frustumPoints(8);

	_ComputeFrustumPoints(fovY, aspect, nearPlane, farPlane, frustumPoints);
	return _CreateCubeMesh(frustumPoints);
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateWireframeFrustum(float fovY, float aspect, float nearPlane, float farPlane)
{
	std::vector< glm::vec3 >	frustumPoints(8);

	_ComputeFrustumPoints(fovY, aspect, nearPlane, farPlane, frustumPoints);
	return _CreateWireframeCubeMesh(frustumPoints);
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateCircle(float radius, int vertexCount)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(vertexCount * 3);
	std::vector< glm::vec3 >				circlePoints(vertexCount);

	_ComputeCirclePoints(radius, vertexCount, circlePoints);
	glm::vec3	center = glm::vec3(0, 0, 0);

	for (int i = 0; i < vertexCount; i++)
	{
		Mesh::VertexAttributes::TriVertexAttrib(
			center, circlePoints[i], circlePoints[(i + 1) % vertexCount],
			attribs.data() + i * 3
		);
	}

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateWireframeCircle(float radius, int vertexCount)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(vertexCount * 2);
	std::vector< glm::vec3 >				circlePoints(vertexCount);

	glm::vec3	normal = glm::vec3(0, 1, 0);
	_ComputeCirclePoints(radius, vertexCount, circlePoints);

	for (int i = 0; i < vertexCount; i++)
	{
		glm::vec3 p0 = circlePoints[i];
		glm::vec3 p1 = circlePoints[(i + 1) % vertexCount];
		attribs[i * 2 + 0] = {p0, normal, glm::normalize(p0), {0, 0, 0}, {0, 0}};
		attribs[i * 2 + 1] = {p1, normal, glm::normalize(p1), {0, 0, 0}, {0, 0}};
	}

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateCone(float radius, float height, int vertexCount)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(vertexCount * 6);
	std::vector< glm::vec3 >				circlePoints(vertexCount);

	_ComputeCirclePoints(radius, vertexCount, circlePoints);
	glm::vec3	peak = glm::vec3(0, height, 0);
	glm::vec3	center = glm::vec3(0, 0, 0);

	for (int i = 0; i < vertexCount; i++)
	{
		Mesh::VertexAttributes::TriVertexAttrib(
			peak, circlePoints[i], circlePoints[(i + 1) % vertexCount],
			attribs.data() + i * 3
		);
	}

	for (int i = 0; i < vertexCount; i++)
	{
		Mesh::VertexAttributes::TriVertexAttrib(
			circlePoints[i], center, circlePoints[(i + 1) % vertexCount],
			attribs.data() + vertexCount * 3 + i * 3
		);
	}

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateWireframeCone(float radius, float height, int vertexCount)
{
	std::shared_ptr< Mesh >					m = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(vertexCount * 4);
	std::vector< glm::vec3 >				circlePoints(vertexCount);

	glm::vec3	normal = glm::vec3(0, 1, 0);
	glm::vec3	peak = glm::vec3(0, height, 0);
	_ComputeCirclePoints(radius, vertexCount, circlePoints);

	for (int i = 0; i < vertexCount; i++)
	{
		glm::vec3 p0 = circlePoints[i];
		glm::vec3 p1 = circlePoints[(i + 1) % vertexCount];
		attribs[i * 4 + 0] = {p0, normal, glm::normalize(p0), {0, 0, 0}, {0, 0}};
		attribs[i * 4 + 1] = {p1, normal, glm::normalize(p1), {0, 0, 0}, {0, 0}};
		attribs[i * 4 + 2] = {p0, normal, glm::normalize(p0), {0, 0, 0}, {0, 0}};
		attribs[i * 4 + 3] = {peak, normal, glm::normalize(p1), {0, 0, 0}, {0, 0}};
	}

	m->SetVertexAttributes(attribs);

	return m;
}

std::shared_ptr< Mesh >		PrimitiveMeshFactory::CreateMesh(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::Cube:
			return _CreateCubeMesh();
		case PrimitiveType::WireframeCube:
			return _CreateWireframeCubeMesh();
		case PrimitiveType::Quad:
			return _CreateQuadMesh();
		case PrimitiveType::WireframeQuad:
			return _CreateWireframeQuadMesh();
		case PrimitiveType::Circle:
			return CreateCircle(1, 100);
		case PrimitiveType::WireframeCircle:
			return CreateWireframeCircle(1, 100);
		case PrimitiveType::Cone:
			return CreateCone(1, 1, 100);
		case PrimitiveType::WireframeCone:
			return CreateWireframeCone(1, 1, 100);
	}
	throw std::runtime_error("Can't find mesh generator for primitive type: " + std::to_string(static_cast< int >(type)));
}


std::ostream &	operator<<(std::ostream & o, PrimitiveMeshFactory const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
