#include "GizmoBase.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC::Gizmo;

static const std::string GizmoShader = "Shaders/Gizmo/Default.hlsl";

GizmoBase::GizmoBase(void)
{
	material = Material::Create(GizmoShader);
	renderer = new MeshRenderer(material);
	AddComponent(renderer);
	color = Color::Red;

	Vk::CreateBuffer(sizeof(LWGC_GizmoData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, gizmoDataBuffer, gizmoDataMemory);
	Vk::UploadToMemory(gizmoDataMemory, &gizmoData, sizeof(LWGC_GizmoData));
}

GizmoBase::GizmoBase(const Color & c) : GizmoBase()
{
	color = c;
}

void	GizmoBase::Initialize(void) noexcept
{
	GameObject::Initialize();

	material->AllocateDescriptorSet("gizmo");
	material->SetBuffer("gizmo", gizmoDataBuffer, sizeof(LWGC_GizmoData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
}

void	GizmoBase::SetColor(const Color & c)
{
	color = c;
	// TODO: update gizmo descriptor for color
}

GizmoBase::~GizmoBase(void)
{
	auto device = VulkanInstance::Get()->GetDevice();

	vkDestroyBuffer(device, gizmoDataBuffer, nullptr);
	vkFreeMemory(device, gizmoDataMemory, nullptr);
}
