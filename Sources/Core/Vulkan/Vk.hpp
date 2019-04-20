#pragma once

#include "VulkanInstance.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include VULKAN_INCLUDE
#include "VulkanInstance.hpp"
#include GLM_INCLUDE

namespace LWGC
{
	class TextureBinding;

	// Helper class for small functions
	class		Vk
	{
		private:

		public:

			class Samplers
			{
				public:
					static VkSampler	trilinearClamp;
					static VkSampler	trilinearRepeat;
					static VkSampler	nearestClamp;
					static VkSampler	nearestRepeat;
					static VkSampler	anisotropicTrilinearClamp;
					static VkSampler	depthCompare;
			};

			Vk(void) = delete;
			Vk(const Vk &) = delete;
			virtual ~Vk(void) = default;

			Vk &	operator=(Vk const & src) = delete;

			static void			Initialize(void);
			static void			Release(void);
			static VkImageView	CreateImageView(VkImage image, VkFormat format, int mipLevels, VkImageViewType viewType, VkImageAspectFlags aspectFlags);
			static void			CreateImage(uint32_t width, uint32_t height, uint32_t depth, int arrayCount, int mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
			static bool			HasStencilComponent(VkFormat format);
			static void			CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
			static void			CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			static VkBufferView	CreateBufferView(VkBuffer buffer, VkFormat format, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);
			static void			CopyBufferToImage(VkBuffer buffer, VkImage image, glm::ivec3 imageSize, glm::ivec3 offset = {0, 0, 0});
			static void			CheckResult(VkResult result, const std::string & errorMessage);
			static VkSampler	CreateSampler(VkFilter filter, VkSamplerAddressMode addressMode, uint32_t maxAniso = 0);
			static VkSampler	CreateCompSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkCompareOp compareOp);
			static VkDescriptorSetLayoutBinding	CreateDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
			static VkDescriptorSetLayoutBinding	CreateDescriptorSetLayoutBinding(TextureBinding binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags);
			static void			CreateDescriptorSetLayout(std::vector< VkDescriptorSetLayoutBinding > bindings, VkDescriptorSetLayout & layout);
			static void			UploadToMemory(VkDeviceMemory memory, void * data, size_t size);

			static void			SetDebugName(const std::string & name, uint64_t vulkanObject, VkDebugReportObjectTypeEXT objectType);
			// Utils functions headers to directly set vulkan objects without specifying the debug object type:
			static void			SetImageDebugName(const std::string & name, VkImage image);
			static void			SetImageViewDebugName(const std::string & name, VkImageView imageView);
			static void			SetCommandBufferDebugName(const std::string & name, VkCommandBuffer cmd);
			static void			SetQueueDebugName(const std::string & name, VkQueue queue);
			static void			SetSamplerDebugName(const std::string & name, VkSampler sampler);
			static void			SetBufferDebugName(const std::string & name, VkBuffer buffer);
			static void			SetDeviceMemoryDebugName(const std::string & name, VkDeviceMemory deviceMemory);
			static void			SetShaderModuleDebugName(const std::string & name, VkShaderModule shaderModule);
			static void			SetPipelineDebugName(const std::string & name, VkPipeline pipeline);
			static void			SetPipelineLayoutDebugName(const std::string & name, VkPipelineLayout pipelineLayout);
			static void			SetRenderPassDebugName(const std::string & name, VkRenderPass renderPass);
			static void			SetFramebufferDebugName(const std::string & name, VkFramebuffer frameBuffer);
			static void			SetDescriptorSetLayoutDebugName(const std::string & name, VkDescriptorSetLayout descriptorSetLayout);
			static void			SetDescriptorSetDebugName(const std::string & name, VkDescriptorSet descriptorSet);
			static void			SetSemaphoreDebugName(const std::string & name, VkSemaphore semaphore);
			static void			SetFenceDebugName(const std::string & name, VkFence fence);
			static void			SetEventDebugName(const std::string & name, VkEvent event);

			static void			BeginProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color);
			static void			InsertProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color);
			static void			EndProfilingSample(VkCommandBuffer cmd);

			static VkFence		CreateFence(bool signaled = false);

			static uint64_t		CreateAccelerationStructure(const VkAccelerationStructureTypeNV type, const uint32_t geometryCount, const VkGeometryNV * geometries, const uint32_t instanceCount);
	};
}