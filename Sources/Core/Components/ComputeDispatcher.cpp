#include "ComputeDispatcher.hpp"

#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Hierarchy.hpp"

using namespace LWGC;

ComputeDispatcher::ComputeDispatcher(std::shared_ptr< Material > material, int width, int height, Texture2D * texture, int depth) : 
	_material(material), _width(width), _height(height), _depth(depth)
{
	_texture = texture;
}

ComputeDispatcher::~ComputeDispatcher(void)
{
	_material.reset();
}

void	ComputeDispatcher::RecordComputeCommand(VkCommandBuffer cmd) noexcept
{
	vkCmdDispatch(cmd, _width, _height, _depth);
}

void	ComputeDispatcher::CreateDescriptorSet(void)
{

}

void			ComputeDispatcher::Initialize(void) noexcept
{
	Component::Initialize();

	VulkanRenderPipeline * renderPipeline = VulkanRenderPipeline::Get();
	
	_material->Initialize(renderPipeline->GetSwapChain(), renderPipeline->GetRenderPass());
	_computeCommandBuffer = VulkanInstance::Get()->GetCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_SECONDARY);

	// Record command buffer:
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

	if (vkBeginCommandBuffer(_computeCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	vkCmdBindPipeline(_computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, _material->GetPipeline());

	std::cout << "Dispatch: " << _width << ", " << _height << ", " << _depth << std::endl;
	vkCmdDispatch(_computeCommandBuffer, _width, _height, _depth);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = _texture->GetImage();
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseMipLevel = 0;

	barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	vkCmdPipelineBarrier(_computeCommandBuffer,
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	if (vkEndCommandBuffer(_computeCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}

	std::cout << "Completed !\n";
}

void			ComputeDispatcher::OnEnable() noexcept
{
	Component::OnEnable();
	_renderContextIndex = hierarchy->RegisterComponentInRenderContext(GetType(), this);
}

void			ComputeDispatcher::OnDisable() noexcept
{
	Component::OnDisable();
	hierarchy->UnregisterComponentInRenderContext(GetType(), _renderContextIndex);
}

std::shared_ptr< Material >	ComputeDispatcher::GetMaterial(void)
{
	return _material;
}

VkCommandBuffer				ComputeDispatcher::GetCommandBuffer(void)
{
	return _computeCommandBuffer;
}

uint32_t		ComputeDispatcher::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::ComputeDispatcher);
}

std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r)
{
	o << "ComputeDispatcher" << std::endl;
	(void)r;
	return (o);
}
