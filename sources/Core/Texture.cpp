#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include STB_INCLUDE_IMAGE

using namespace LWGC;

Texture::Texture(void) : width(0), height(0), depth(1), arraySize(1), autoGenerateMips(false), usage(0), allocated(false)
{
}

Texture::Texture(Texture const & src)
{
	*this = src;
}

Texture::~Texture(void)
{
}

Texture &	Texture::operator=(Texture const & src)
{
	if (this != &src)
	{
		this->width = src.width;
		this->height = src.height;
		this->autoGenerateMips = src.autoGenerateMips;
		this->view = src.view;
	}
	return (*this);
}

void			Texture::AllocateImage(void)
{
	this->allocated = true;
	
	Vk::CreateImage(width, height, depth, arraySize, format, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);
	view = Vk::CreateImageView(image, format, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
}

// TODO: HDR and EXR support (stbi_us)
stbi_uc *		Texture::LoadFromFile(const std::string & fileName, int & width, int & height)
{
	int texChannels;
	stbi_uc *pixels = stbi_load(fileName.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);

	if (!pixels)
		throw std::runtime_error("failed to load texture image!");

	return pixels;
}

void			Texture::UploadImage(stbi_uc * pixels, VkDeviceSize imageSize, int targetArrayIndex, int targetMipLevel)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	VkDevice device = VulkanInstance::Get()->GetDevice();
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Vk::CopyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height));
	TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void		Texture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	const auto graphicCommandBufferPool = VulkanInstance::Get()->GetGraphicCommandBufferPool();
	VkCommandBuffer commandBuffer = graphicCommandBufferPool->BeginSingle();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        	);

	graphicCommandBufferPool->EndSingle(commandBuffer);
}

int				Texture::GetWidth(void) const noexcept { return (this->width); }
int				Texture::GetHeight(void) const noexcept { return (this->height); }
int				Texture::GetDepth(void) const noexcept { return (this->depth); }
VkImageView		Texture::GetView(void) const noexcept { return this->view; }
VkImage			Texture::GetImage(void) const noexcept { return this->image; }
bool			Texture::GetAutoGenerateMips(void) const noexcept { return this->autoGenerateMips; }

std::ostream &	operator<<(std::ostream & o, Texture const & r)
{
	o << "Texture" << std::endl;
	(void)r;
	return (o);
}
