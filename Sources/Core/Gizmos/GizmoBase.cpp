#include "GizmoBase.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC::Gizmo;

static const std::string GizmoShader = "Shaders/Gizmo/Default.hlsl";

GizmoBase::GizmoBase(const Color & c)
{
	material = Material::Create(GizmoShader);
	renderer = new MeshRenderer(material);
	AddComponent(renderer);

	gizmoData.color = c;
	gizmoData.colorMode = 0; // TODO: implement color mode

	Vk::CreateBuffer(sizeof(LWGC_GizmoData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, gizmoDataBuffer, gizmoDataMemory);
	Vk::UploadToMemory(gizmoDataMemory, &gizmoData, sizeof(LWGC_GizmoData));
}

void	GizmoBase::Initialize(void) noexcept
{
	GameObject::Initialize();

	material->AllocateDescriptorSet("gizmo");
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
