#include "Circle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Circle::Circle(float radius, int vertices, bool wireframe, const Color & c) : GizmoBase(c, wireframe)
{
	std::shared_ptr< Mesh >	circleMesh;

	if (wireframe)
		circleMesh = PrimitiveMeshFactory::CreateWireframeCircle(radius, vertices);
	else
		circleMesh = PrimitiveMeshFactory::CreateCircle(radius, vertices);

	renderer->SetMesh(circleMesh);
}

Circle::~Circle(void)
{
}

std::ostream &	operator<<(std::ostream & o, Circle const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
