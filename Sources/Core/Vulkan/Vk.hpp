#pragma once

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE
#include "VulkanInstance.hpp"

namespace LWGC
{
	enum class TextureBinding;
	
	// Helper class for small functions
	class		Vk
	{
		private:
	
		public:

			// TODO: remove this !
			static VkPipelineLayout	currentPipelineLayout;

			class Samplers
			{
				public:
					static VkSampler	trilinearClamp;
					static VkSampler	trilinearRepeat;
					static VkSampler	nearestClamp;
					static VkSampler	nearestRepeat;
					static VkSampler	depthCompare;
			};

			Vk(void) = delete;
			Vk(const Vk &) = delete;
			virtual ~Vk(void) = default;
	
			Vk &	operator=(Vk const & src) = delete;

			static void			Initialize(void);
			static VkImageView	CreateImageView(VkImage image, VkFormat format, int mipLevels, VkImageViewType viewType, VkImageAspectFlags aspectFlags);
			static void			CreateImage(uint32_t width, uint32_t height, uint32_t depth, int arrayCount, int mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
			static bool			HasStencilComponent(VkFormat format);
			static void			CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
			static void			CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			static void			CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t depth = 1);
			static void			CheckResult(VkResult result, const std::string & errorMessage);
			static VkSampler	CreateSampler(VkFilter filter, VkSamplerAddressMode addressMode, uint32_t maxAniso = 0);
			static VkSampler	CreateCompSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkCompareOp compareOp);
			static VkDescriptorSetLayoutBinding	CreateDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
			static VkDescriptorSetLayoutBinding	CreateDescriptorSetLayoutBinding(TextureBinding binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
			static void			CreateDescriptorSetLayout(std::vector< VkDescriptorSetLayoutBinding > bindings, VkDescriptorSetLayout & layout);
	};
}