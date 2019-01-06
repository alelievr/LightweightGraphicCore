#include "Line.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Line::Line(const glm::vec3 & p0, const glm::vec3 & p1, const Color & c) : GizmoBase(c, true), _vertexAttributes(2)
{
	_lineMesh = std::make_shared< Mesh >();
	Mesh::VertexAttributes::EdgeVertexAttrib(p0, p1, _vertexAttributes.data());

	_lineMesh->SetVertexAttributes(_vertexAttributes);
	renderer->SetMesh(_lineMesh);
}

Line::~Line(void)
{
}

// TODO: fix this
void	Line::SetPoint0(const glm::vec3 p)
{
	_vertexAttributes[0].position = p;
	_lineMesh->SetVertexAttributes(_vertexAttributes);
	_lineMesh->UploadDatas();
}
void	Line::SetPoint1(const glm::vec3 p)
{
	_vertexAttributes[1].position = p;
	_lineMesh->SetVertexAttributes(_vertexAttributes);
	_lineMesh->UploadDatas();
}

std::ostream &	operator<<(std::ostream & o, Line const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
