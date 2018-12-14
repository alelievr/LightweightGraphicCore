#include "CommandBufferPool.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

CommandBufferPool::CommandBufferPool(void)
{
	this->_instance = VK_NULL_HANDLE;
	this->_commandPool = VK_NULL_HANDLE;
	this->_queueIndex = -1;
}

CommandBufferPool::~CommandBufferPool(void)
{
	if (_commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(_instance->GetDevice(), _commandPool, nullptr);
		_commandPool = VK_NULL_HANDLE;
	}
}

void				CommandBufferPool::Initialize(void)
{
	_instance = VulkanInstance::Get();
	_queue = _instance->GetQueue();
	_queueIndex = _instance->GetQueueIndex();

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = _queueIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(_instance->GetDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
		throw std::runtime_error("Failed to create command pool !");
}

void			CommandBufferPool::Allocate(VkCommandBufferLevel level, std::vector< VkCommandBuffer > & commandBuffers, size_t count)
{
	if (commandBuffers.size() != count)
		commandBuffers.resize(count);

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _commandPool;
	allocInfo.level = level;
	allocInfo.commandBufferCount = count;

	if (vkAllocateCommandBuffers(_instance->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers !");
}

VkCommandBuffer		CommandBufferPool::Allocate(VkCommandBufferLevel level)
{
	std::vector< VkCommandBuffer > cmds;

	Allocate(level, cmds, 1);

	return cmds[0];
}

void				CommandBufferPool::FreeCommandBuffers(std::vector< VkCommandBuffer > commandBuffers) noexcept
{
	vkFreeCommandBuffers(_instance->GetDevice(), _commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

VkCommandBuffer		CommandBufferPool::BeginSingle(VkCommandBufferLevel level) noexcept
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = level;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	Vk::CheckResult(vkAllocateCommandBuffers(_instance->GetDevice(), &allocInfo, &commandBuffer), "Allocate commnand buffer failed");

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	Vk::CheckResult(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Begin command buffer failed");

	return commandBuffer;
}

void		CommandBufferPool::EndSingle(VkCommandBuffer commandBuffer) noexcept
{
	Vk::CheckResult(vkEndCommandBuffer(commandBuffer), "End command buffer failed");

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	Vk::CheckResult(vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE), "Queue submit failed");
	Vk::CheckResult(vkQueueWaitIdle(_queue), "QueueWait Idle failed");

	vkFreeCommandBuffers(_instance->GetDevice(), _commandPool, 1, &commandBuffer);
}

std::ostream &	operator<<(std::ostream & o, CommandBufferPool const & r)
{
	o << "Command Buffer Pool" << std::endl;
	(void)r;
	return (o);
}
