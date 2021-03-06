#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class	VulkanInstance;

	class		CommandBufferPool
	{
		private:
			VulkanInstance *				_instance;
			uint32_t						_queueIndex;
			VkQueue							_queue;
			std::vector< VkCommandBuffer >	_frameCommandBuffers;

		public:
			VkCommandPool					_commandPool;
			CommandBufferPool(void);
			CommandBufferPool(const CommandBufferPool &) = delete;
			virtual ~CommandBufferPool(void);

			CommandBufferPool &	operator=(CommandBufferPool const & src) = delete;

			void			AllocateFrameCommandBuffers(size_t frameBufferCount);
			void			Initialize(VkQueue queue, int queueIndex);
			VkCommandBuffer	Allocate(VkCommandBufferLevel level);
			void			Allocate(VkCommandBufferLevel level, std::vector< VkCommandBuffer > & commandBuffers, size_t count);
			void			FreeCommandBuffers(std::vector< VkCommandBuffer > commandBuffers) noexcept;
			VkCommandBuffer	GetFrameCommandBuffer(size_t frameIndex);
			void			ResetCommandBuffer(size_t frameIndex);

			VkCommandBuffer	BeginSingle(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) noexcept;
			void			EndSingle(VkCommandBuffer commandBuffer, VkFence fence = VK_NULL_HANDLE) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, CommandBufferPool const & r);
}

#include "VulkanInstance.hpp"
