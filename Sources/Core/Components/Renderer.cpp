#include "Renderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"


#include "IncludeDeps.hpp"
#include GLM_INCLUDE

using namespace LWGC;

VkDescriptorSetLayout	Renderer::_descriptorSetLayout = VK_NULL_HANDLE;

Renderer::Renderer(void)
{
	_material = std::make_shared< Material >();
}

Renderer::Renderer(std::shared_ptr< Material > material)
{
	_material = material;
}

Renderer::~Renderer(void)
{
	_material.reset();
	vkDestroyBuffer(device, _uniformModelBuffer.buffer, nullptr);
	vkFreeMemory(device, _uniformModelBuffer.memory, nullptr);
}

void		Renderer::Initialize(void) noexcept
{
	Component::Initialize();

	VulkanRenderPipeline * renderPipeline = VulkanRenderPipeline::Get();

	_material->Initialize(renderPipeline->GetSwapChain(), renderPipeline->GetRenderPass());

	_drawCommandBuffer = VulkanInstance::Get()->GetCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_SECONDARY);

	if (_descriptorSetLayout == VK_NULL_HANDLE)
		CreateGraphicDescriptorSetLayout();

	CreateDescriptorSet();
	RecordDrawCommandBuffer();
}

void		Renderer::CreateGraphicDescriptorSetLayout(void) noexcept
{
	auto binding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
	Vk::CreateDescriptorSetLayout({binding}, _descriptorSetLayout);
}

void		Renderer::CreateDescriptorSet(void)
{
	Vk::CreateBuffer(
		sizeof(LWGC_PerObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformModelBuffer.buffer,
		_uniformModelBuffer.memory
	);

	std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = VulkanInstance::Get()->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(device, &allocInfo, &_descriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformModelBuffer.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerObject);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
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

	void* data;
	vkMapMemory(device, _uniformModelBuffer.memory, 0, sizeof(LWGC_PerObject), 0, &data);
	memcpy(data, &_perObject, sizeof(_perObject));
	vkUnmapMemory(device, _uniformModelBuffer.memory);
}

void		Renderer::RecordDrawCommandBuffer(void)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

	if (vkBeginCommandBuffer(_drawCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	RecordDrawCommand(_drawCommandBuffer);

	if (vkEndCommandBuffer(_drawCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

Bounds		Renderer::GetBounds(void)
{
	std::cout << "GetBounds(): TODO" << std::endl;
	return Bounds();
}

void Renderer::OnEnable() noexcept
{
	Component::OnEnable();
	_renderContextIndex = hierarchy->RegisterComponentInRenderContext(GetType(), this);
}

void Renderer::OnDisable() noexcept
{
	Component::OnDisable();
	hierarchy->UnregisterComponentInRenderContext(GetType(), _renderContextIndex);
}

void Renderer::CleanupPipeline(void) noexcept
{
	_material->CleanupPipeline();
}

void Renderer::CreatePipeline(void) noexcept
{
	_material->CreatePipeline();
}

std::shared_ptr< Material >	Renderer::GetMaterial(void) const { return (this->_material); }
void						Renderer::SetMaterial(std::shared_ptr< Material > tmp) { this->_material = tmp; }

VkCommandBuffer				Renderer::GetDrawCommandBuffer(void) const { return _drawCommandBuffer; }
VkDescriptorSet				Renderer::GetDescriptorSet(void) const { return _descriptorSet; }

std::ostream &	operator<<(std::ostream & o, Renderer const & r)
{
	o << "Renderer" << std::endl;
	(void)r;
	return (o);
}
