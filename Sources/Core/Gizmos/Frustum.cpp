#include "Frustum.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

#include "Utils/Math.hpp"

using namespace LWGC;
using namespace Gizmo;

Frustum::Frustum(float fovY, float aspect, float nearPlane, float farPlane, bool wireframe, const Color & c) : GizmoBase(c, wireframe)
{
	std::shared_ptr< Mesh >	frustumMesh;

	if (wireframe)
		frustumMesh = PrimitiveMeshFactory::CreateWireframeFrustum(fovY, aspect, nearPlane, farPlane);
	else
		frustumMesh = PrimitiveMeshFactory::CreateFrustum(fovY, aspect, nearPlane, farPlane);

	renderer->SetMesh(frustumMesh);
}

Frustum::~Frustum(void)
{
}

// TODO: update mesh and upload to device
void	Frustum::SetPoint0(const glm::vec3 p) { _p0 = p; }
void	Frustum::SetPoint1(const glm::vec3 p) { _p1 = p; }

std::ostream &	operator<<(std::ostream & o, Frustum const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
