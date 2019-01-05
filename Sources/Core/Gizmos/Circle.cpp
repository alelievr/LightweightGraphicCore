#include "Circle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Circle::Circle(float radius, int vertices, bool wireframe, const Color & c) : GizmoBase(c)
{
	std::shared_ptr< Mesh >	circleMesh;

	if (wireframe)
		circleMesh = PrimitiveMeshFactory::CreateWireframeCircle(radius, vertices);
	else
		circleMesh = PrimitiveMeshFactory::CreateCircle(radius, vertices);

	renderer->SetMesh(circleMesh);

	VkPipelineRasterizationStateCreateInfo rasterState = {};
	rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterState.depthClampEnable = VK_FALSE;
	rasterState.rasterizerDiscardEnable = VK_FALSE;
	rasterState.polygonMode = (wireframe) ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	rasterState.lineWidth = 1.0f;
	rasterState.cullMode = VK_CULL_MODE_NONE;
	rasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterState.depthBiasEnable = VK_FALSE;

	VkPipelineInputAssemblyStateCreateInfo assemblyState = {};
	assemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyState.topology = (wireframe) ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST : VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assemblyState.primitiveRestartEnable = VK_FALSE;

	material->SetRasterizationState(rasterState);
	material->SetInputAssemblyState(assemblyState);
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
