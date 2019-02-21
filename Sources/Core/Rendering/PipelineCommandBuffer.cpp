#include "Core/Rendering/PipelineCommandBuffer.hpp"

#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

void				PipelineCommandBuffer::Allocate(size_t commandBufferCount)
{
	VulkanInstance::Get()->GetCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_SECONDARY, _commandBuffers, commandBufferCount);
}

VkCommandBuffer		PipelineCommandBuffer::GetCommandBuffer(size_t frameIndex) const
{
	return _commandBuffers[frameIndex];
}