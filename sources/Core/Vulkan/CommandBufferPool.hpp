#pragma once

#include <iostream>
#include <string>

#include "vulkan/vulkan.h"

class	VulkanInstance;

enum class CommandBufferQueue
{
	Graphic,
	Compute,
};

class		CommandBufferPool
{
	private:
		VulkanInstance *				_instance;
		VkCommandPool					_commandPool;
		uint32_t						_queueIndex;
		VkQueue							_queue;

	public:
		CommandBufferPool(void);
		CommandBufferPool(const CommandBufferPool &) = delete;
		virtual ~CommandBufferPool(void);

		CommandBufferPool &	operator=(CommandBufferPool const & src) = delete;

		void			Initialize(CommandBufferQueue queueType);
		VkCommandBuffer	Allocate(VkCommandBufferLevel level);
		void			Allocate(VkCommandBufferLevel level, std::vector< VkCommandBuffer > & commandBuffers, size_t count);
		void			FreeCommandBuffers(std::vector< VkCommandBuffer > commandBuffers) noexcept;

		VkCommandBuffer	BeginSingle(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) noexcept;
		void			EndSingle(VkCommandBuffer commandBuffer) noexcept;
};

#include "VulkanInstance.hpp"

std::ostream &	operator<<(std::ostream & o, CommandBufferPool const & r);
