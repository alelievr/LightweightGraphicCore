#include "GizmoBase.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC::Gizmo;

static const std::string GizmoShader = "Shaders/Gizmo/Default.hlsl";

GizmoBase::GizmoBase(const Color & c, bool wireframe)
{
	// TODO: share the gizmo material to avoid useless descriptor allocation
	material = Material::Create(GizmoShader);
	renderer = new MeshRenderer(material);
	AddComponent(renderer);

	gizmoData.color = c;
	gizmoData.colorMode = 0; // TODO: implement color mode

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

void	GizmoBase::Initialize(void) noexcept
{
	GameObject::Initialize();

	Vk::CreateBuffer(sizeof(LWGC_GizmoData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, gizmoDataBuffer, gizmoDataMemory);
	Vk::UploadToMemory(gizmoDataMemory, &gizmoData, sizeof(LWGC_GizmoData));

	material->SetBuffer("gizmo", gizmoDataBuffer, sizeof(LWGC_GizmoData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
}

void	GizmoBase::SetColor(const Color & c)
{
	gizmoData.color = c;
	Vk::UploadToMemory(gizmoDataMemory, &gizmoData, sizeof(LWGC_GizmoData));
}

GizmoBase::~GizmoBase(void)
{
	auto device = VulkanInstance::Get()->GetDevice();

	vkDestroyBuffer(device, gizmoDataBuffer, nullptr);
	vkFreeMemory(device, gizmoDataMemory, nullptr);
}
