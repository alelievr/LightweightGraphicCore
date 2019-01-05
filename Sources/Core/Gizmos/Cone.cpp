#include "Cone.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Cone::Cone(float radius, float height, int vertices, bool wireframe, const Color & c) : GizmoBase(c, wireframe)
{
	std::shared_ptr< Mesh >	coneMesh;

	if (wireframe)
		coneMesh = PrimitiveMeshFactory::CreateWireframeCone(radius, height, vertices);
	else
		coneMesh = PrimitiveMeshFactory::CreateCone(radius, height, vertices);

	renderer->SetMesh(coneMesh);
}

Cone::~Cone(void)
{
}

std::ostream &	operator<<(std::ostream & o, Cone const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
