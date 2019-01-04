#include "Line.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Line::Line(const glm::vec3 p0, const glm::vec3 p1, const Color & c) : GizmoBase(c), _p0(p0), _p1(p1)
{
	std::shared_ptr< Mesh >					lineMesh = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(4);

	lineMesh->SetVertexAttributes(attribs);
	lineMesh->SetIndices({0, 1, 2, 2, 3, 0});

	lineMesh->SetVertexAttributes({
		{p0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)},
		{p1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)}
	});

	renderer->SetMesh(lineMesh);

	VkPipelineRasterizationStateCreateInfo rasterState = {};
	rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterState.depthClampEnable = VK_FALSE;
	rasterState.rasterizerDiscardEnable = VK_FALSE;
	rasterState.polygonMode = VK_POLYGON_MODE_LINE;
	rasterState.lineWidth = 1.0f;
	rasterState.cullMode = VK_CULL_MODE_NONE;
	rasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterState.depthBiasEnable = VK_FALSE;

	material->SetRasterizationState(rasterState);
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
