#include "MeshRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

VkDescriptorSetLayout	MeshRenderer::_descriptorSetLayout = VK_NULL_HANDLE;

MeshRenderer::MeshRenderer(void) : _initDescriptorSetLayout(false)
{
	_mesh = std::make_shared< Mesh >();
	_material = std::make_shared< Material >();
}

MeshRenderer::MeshRenderer(const PrimitiveType prim) : _initDescriptorSetLayout(false)
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
	_material = std::make_shared< Material >();
}

MeshRenderer::~MeshRenderer(void)
{
}

void		MeshRenderer::Initialize(void) noexcept
{
	Component::Initialize();

	VulkanRenderPipeline * renderPipeline = VulkanRenderPipeline::Get();

	_material->Initialize(renderPipeline->GetSwapChain(), renderPipeline->GetRenderPass());
	_mesh->UploadDatas();

	_drawCommandBuffer = VulkanInstance::Get()->GetGraphicCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_SECONDARY);

	auto binding = Vk::CreateDescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_ALL_GRAPHICS);
	Vk::CreateDescriptorSetLayout({binding}, _descriptorSetLayout);

	const auto device = VulkanInstance::Get()->GetDevice();
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


	RecordDrawCommandBuffer();
}

void		MeshRenderer::RecordDrawCommandBuffer(void)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

	if (vkBeginCommandBuffer(_drawCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	vkCmdBindPipeline(_drawCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetGraphicPipeline());

	_mesh->BindBuffers(_drawCommandBuffer);

	_material->BindDescriptorSets(_drawCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
	_mesh->Draw(_drawCommandBuffer);

	if (vkEndCommandBuffer(_drawCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

Bounds		MeshRenderer::GetBounds(void)
{
	// TODO
	return Bounds();
}

void		MeshRenderer::SetModel(const Mesh & mesh, const Material & material)
{
	this->_mesh = std::make_shared< Mesh >(mesh);
	this->_material = std::make_shared< Material >(material);
}

void		MeshRenderer::SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material)
{
	this->_mesh = mesh;
	this->_material = material;
}

void MeshRenderer::OnEnable() noexcept
{
	Component::OnEnable();
	_renderContextIndex = hierarchy->RegisterComponentInRenderContext(MeshRenderer::type, this);
}

void MeshRenderer::OnDisable() noexcept
{
	Component::OnEnable();
	hierarchy->UnregisterComponentInRenderContext(MeshRenderer::type, _renderContextIndex);
}

void MeshRenderer::CleanupGraphicPipeline(void) noexcept
{
	_material->CleanupGraphicPipeline();
}

void MeshRenderer::CreateGraphicPipeline(void) noexcept
{
	_material->CreateGraphicPipeline();
}

uint32_t					MeshRenderer::GetType(void) const noexcept { return type; }

std::shared_ptr< Mesh >		MeshRenderer::GetMesh(void) const { return (this->_mesh); }
void		MeshRenderer::SetMesh(std::shared_ptr< Mesh > tmp) { this->_mesh = tmp; }

std::shared_ptr< Material >		MeshRenderer::GetMaterial(void) const { return (this->_material); }
void		MeshRenderer::SetMaterial(std::shared_ptr< Material > tmp) { this->_material = tmp; }

VkCommandBuffer		MeshRenderer::GetDrawCommandBuffer(void) const { return _drawCommandBuffer; }

VkDescriptorSetLayout	MeshRenderer::GetDescriptorSetLayout(void) noexcept { return _descriptorSetLayout; }

std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
