#include "Vk.hpp"

#include "Core/Vulkan/Material.hpp"

using namespace LWGC;

VkSampler Vk::Samplers::depthCompare;
VkSampler Vk::Samplers::trilinearClamp;
VkSampler Vk::Samplers::trilinearRepeat;
VkSampler Vk::Samplers::nearestClamp;
VkSampler Vk::Samplers::nearestRepeat;
VkSampler Vk::Samplers::anisotropicTrilinearClamp;

VkImageView		Vk::CreateImageView(VkImage image, VkFormat format, int mipLevels, VkImageViewType viewType, VkImageAspectFlags aspectFlags)
{
	VulkanInstance * instance = VulkanInstance::Get();

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = viewType;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	auto device = instance->GetDevice();
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void			Vk::CreateImage(uint32_t width, uint32_t height, uint32_t depth, int arrayCount, int mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VulkanInstance * instance = VulkanInstance::Get();

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = depth;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = arrayCount;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;

	auto device = instance->GetDevice();
	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	    throw std::runtime_error("failed to create image!");

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = instance->FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	    throw std::runtime_error("failed to allocate image memory!");

	CheckResult(vkBindImageMemory(device, image, imageMemory, 0), "Bind image memory failed");
}

bool			Vk::HasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void			Vk::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory)
{
	VulkanInstance * instance = VulkanInstance::Get();
	const auto & device = instance->GetDevice();

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer!");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = instance->FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory!");

	CheckResult(vkBindBufferMemory(device, buffer, bufferMemory, 0), "Bind Buffer Memory failed");
}

void			Vk::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	const auto & graphicCommandBufferPool = VulkanInstance::Get()->GetCommandBufferPool();
	VkCommandBuffer commandBuffer = graphicCommandBufferPool->BeginSingle();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	graphicCommandBufferPool->EndSingle(commandBuffer);
}

void			Vk::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t depth)
{
	const auto & graphicCommandBufferPool = VulkanInstance::Get()->GetCommandBufferPool();
	VkCommandBuffer commandBuffer = graphicCommandBufferPool->BeginSingle();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {
		width,
		height,
		depth
	};

	vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
			);

	graphicCommandBufferPool->EndSingle(commandBuffer);
}

void			Vk::CheckResult(VkResult result, const std::string & errorMessage)
{
	if (result == 0)
		return;

	std::cout << "Vulkan error [" << result << "]: " << errorMessage << std::endl;

    if (result < 0)
		abort();
}

VkSampler		Vk::CreateSampler(VkFilter filter, VkSamplerAddressMode addressMode, uint32_t maxAniso)
{
	VkSampler			sampler;
	VkSamplerCreateInfo	samplerInfo = {};

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = filter;
	samplerInfo.minFilter = filter;
	samplerInfo.addressModeU = addressMode;
	samplerInfo.addressModeV = addressMode;
	samplerInfo.addressModeW = addressMode;
	samplerInfo.anisotropyEnable = maxAniso != 0;
	samplerInfo.maxAnisotropy = maxAniso;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 16.0f;

	if (vkCreateSampler(VulkanInstance::Get()->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture sampler!");

	return sampler;
}

VkSampler	Vk::CreateCompSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkCompareOp compareOp)
{

	VkSampler			sampler;
	VkSamplerCreateInfo	samplerInfo = {};

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = filter;
	samplerInfo.minFilter = filter;
	samplerInfo.addressModeU = addressMode;
	samplerInfo.addressModeV = addressMode;
	samplerInfo.addressModeW = addressMode;
	samplerInfo.anisotropyEnable = false;
	samplerInfo.maxAnisotropy = 0;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_TRUE;
	samplerInfo.compareOp = compareOp;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 16.0f;

	if (vkCreateSampler(VulkanInstance::Get()->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture sampler!");

	return sampler;
}

void			Vk::Initialize(void)
{
	Samplers::trilinearClamp = CreateSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0);
	Samplers::trilinearRepeat = CreateSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0);
	Samplers::nearestClamp = CreateSampler(VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0);
	Samplers::nearestRepeat = CreateSampler(VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0);
	Samplers::anisotropicTrilinearClamp = CreateSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 16);
	Samplers::depthCompare = CreateCompSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_COMPARE_OP_LESS);
}

void			Vk::Release(void)
{
	VkDevice	device = VulkanInstance::Get()->GetDevice();

	vkDestroySampler(device, Samplers::trilinearClamp, nullptr);
	vkDestroySampler(device, Samplers::trilinearRepeat, nullptr);
	vkDestroySampler(device, Samplers::nearestClamp, nullptr);
	vkDestroySampler(device, Samplers::nearestRepeat, nullptr);
	vkDestroySampler(device, Samplers::anisotropicTrilinearClamp, nullptr);
	vkDestroySampler(device, Samplers::depthCompare, nullptr);
}

VkDescriptorSetLayoutBinding	Vk::CreateDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlagBits stageFlags)
{
	// LWGC per frame cbuffer layout
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = binding;
	layoutBinding.descriptorCount = 1;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = stageFlags;

	return layoutBinding;
}

void			Vk::CreateDescriptorSetLayout(std::vector< VkDescriptorSetLayoutBinding > bindings, VkDescriptorSetLayout & layout)
{
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(VulkanInstance::Get()->GetDevice(), &layoutInfo, nullptr, &layout) != VK_SUCCESS)
	    throw std::runtime_error("failed to create descriptor set layout!");
}

void			Vk::UploadToMemory(VkDeviceMemory memory, void * data, size_t size)
{
	void *		tmpData;
	VkDevice	device = VulkanInstance::Get()->GetDevice();

	vkMapMemory(device, memory, 0, size, 0, &tmpData);
	memcpy(tmpData, data, size);
	vkUnmapMemory(device, memory);
}