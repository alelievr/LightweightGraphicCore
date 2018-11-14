#pragma once

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE
#include "VulkanInstance.hpp"

namespace LWGC
{
	// Helper class for small functions
	class		Vk
	{
		private:
	
		public:
			Vk(void) = delete;
			Vk(const Vk &) = delete;
			virtual ~Vk(void) = default;
	
			Vk &	operator=(Vk const & src) = delete;
	
			static void			SetVulkanInstance(VulkanInstance * instance);
			static VkImageView	CreateImageView(VkImage image, VkFormat format, VkImageViewType viewType, VkImageAspectFlags aspectFlags);
			static void			CreateImage(uint32_t width, uint32_t height, uint32_t depth, int arrayCount, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
			static bool			HasStencilComponent(VkFormat format);
			static void			CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
			static void			CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			static void			CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
			static void			CheckResult(VkResult result);
	};
}