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
			VkCommandPool					_commandPool;
			uint32_t						_queueIndex;
			VkQueue							_queue;
	
		public:
			CommandBufferPool(void);
			CommandBufferPool(const CommandBufferPool &) = delete;
			virtual ~CommandBufferPool(void);
	
			CommandBufferPool &	operator=(CommandBufferPool const & src) = delete;
	
			void			Initialize(void);
			VkCommandBuffer	Allocate(VkCommandBufferLevel level);
			void			Allocate(VkCommandBufferLevel level, std::vector< VkCommandBuffer > & commandBuffers, size_t count);
			void			FreeCommandBuffers(std::vector< VkCommandBuffer > commandBuffers) noexcept;
	
			VkCommandBuffer	BeginSingle(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) noexcept;
			void			EndSingle(VkCommandBuffer commandBuffer) noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, CommandBufferPool const & r);
}	

#include "VulkanInstance.hpp"
