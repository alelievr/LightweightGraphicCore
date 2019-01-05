#include "Arrow.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"

using namespace LWGC;
using namespace Gizmo;

Arrow::Arrow(float length, int vertices, bool wireframe, const Color & c) : Cone(length / 12.0f, length / 4.0f, vertices, wireframe, c)
{
	glm::vec3								coneStartPosition = glm::vec3(0, 3.0 / 4.0 * length, 0);
	std::shared_ptr< Mesh >					lineMesh = std::make_shared< Mesh >();
	std::vector< Mesh::VertexAttributes >	attribs(2);

	// move the cone of 3/4 of the arrow length
	renderer->GetMesh()->Translate(coneStartPosition);

	lineMaterial = Material::Create("Shaders/Gizmo/Default.hlsl");
	lineRenderer = new MeshRenderer(lineMaterial);
	AddComponent(lineRenderer);

	Mesh::VertexAttributes::EdgeVertexAttrib(glm::vec3(0, 0, 0), coneStartPosition, attribs.data());

	lineMesh->SetVertexAttributes(attribs);
	lineRenderer->SetMesh(lineMesh);

	VkPipelineRasterizationStateCreateInfo rasterState = {};
	rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterState.depthClampEnable = VK_FALSE;
	rasterState.rasterizerDiscardEnable = VK_FALSE;
	rasterState.polygonMode = VK_POLYGON_MODE_LINE;
	rasterState.lineWidth = 1.0f;
	rasterState.cullMode = VK_CULL_MODE_NONE;
	rasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterState.depthBiasEnable = VK_FALSE;

	VkPipelineInputAssemblyStateCreateInfo assemblyState = {};
	assemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyState.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	assemblyState.primitiveRestartEnable = VK_FALSE;

	lineMaterial->SetRasterizationState(rasterState);
	lineMaterial->SetInputAssemblyState(assemblyState);
}

void	Arrow::Initialize(void) noexcept
{
	Cone::Initialize();
	lineMaterial->AllocateDescriptorSet("gizmo");
	lineMaterial->SetBuffer("gizmo", gizmoDataBuffer, sizeof(LWGC_GizmoData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
}

Arrow::~Arrow(void)
{
}

std::ostream &	operator<<(std::ostream & o, Arrow const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
