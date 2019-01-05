#include "Line.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Line::Line(const glm::vec3 p0, const glm::vec3 p1, const Color & c) : GizmoBase(c, true), _p0(p0), _p1(p1)
{
	std::shared_ptr< Mesh >					lineMesh = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(2);

	Mesh::VertexAttributes::EdgeVertexAttrib(p0, p1, attribs.data());

	lineMesh->SetVertexAttributes(attribs);
	renderer->SetMesh(lineMesh);
}

Line::~Line(void)
{
}

// TODO: update mesh and upload to device
void	Line::SetPoint0(const glm::vec3 p) { _p0 = p; }
void	Line::SetPoint1(const glm::vec3 p) { _p1 = p; }

std::ostream &	operator<<(std::ostream & o, Line const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
