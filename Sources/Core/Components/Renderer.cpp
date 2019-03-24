#include "Renderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/RenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Utils/Vector.hpp"
#include "Core/Application.hpp"

#include "IncludeDeps.hpp"
#include GLM_INCLUDE

using namespace LWGC;

Renderer::Renderer(void)
{
	_material = Material::Create();
}

Renderer::Renderer(Material * material)
{
	_material = material;
}

Renderer::~Renderer(void)
{
	vkDestroyBuffer(device, _uniformModelBuffer.buffer, nullptr);
	vkFreeMemory(device, _uniformModelBuffer.memory, nullptr);
}

void		Renderer::Initialize(void) noexcept
{
	Component::Initialize();

	_material->MarkAsReady();

	Vk::CreateBuffer(
		sizeof(LWGC_PerObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformModelBuffer.buffer,
		_uniformModelBuffer.memory
	);

	_perRendererSet.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _uniformModelBuffer.buffer, sizeof(LWGC_PerObject));
}

void		Renderer::Update(void) noexcept
{
	// TODO: check for transform changes before to reupload the uniform datas
	UpdateUniformData();
}

void		Renderer::UpdateUniformData(void)
{
	_perObject.model = transform->GetLocalToWorldMatrix();

	// Transpose for HLSL
	_perObject.model = glm::transpose(_perObject.model);

	Vk::UploadToMemory(_uniformModelBuffer.memory, &_perObject, sizeof(_perObject));
}

Bounds		Renderer::GetBounds(void) noexcept
{
	return Bounds();
}

void		Renderer::OnEnable() noexcept
{
	Component::OnEnable();
	_renderContextIndex = hierarchy->RegisterComponentInRenderContext(GetType(), this);
}

void		Renderer::OnDisable() noexcept
{
	Component::OnDisable();
	hierarchy->UnregisterComponentInRenderContext(GetType(), _renderContextIndex);
}

void		Renderer::RecordCommands(VkCommandBuffer cmd)
{
	RecordDrawCommand(cmd);
}

Material *	Renderer::GetMaterial(void) { return (this->_material); }
void						Renderer::SetMaterial(Material * tmp) { this->_material = tmp; }

VkDescriptorSet				Renderer::GetDescriptorSet(void) { return _perRendererSet.GetDescriptorSet(); }

std::ostream &	operator<<(std::ostream & o, Renderer const & r)
{
	o << "Renderer" << std::endl;
	(void)r;
	return (o);
}
