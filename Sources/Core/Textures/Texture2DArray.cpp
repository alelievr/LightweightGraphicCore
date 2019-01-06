#include "Texture2DArray.hpp"

#include <string.h>

using namespace LWGC;

#include STB_INCLUDE_IMAGE

Texture2DArray::Texture2DArray(int width, int height, int arraySize, VkFormat format, int usage)
{
	this->format = format;
	this->width = width;
	this->height = height;
	this->arraySize = arraySize;
	this->usage = usage;

	AllocateImage(VK_IMAGE_VIEW_TYPE_2D_ARRAY);

	// TODO: hardcoded pixel size
	VkDeviceSize imageSize = width * height * 4 * arraySize;
	Vk::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingBufferMemory);
}

void	Texture2DArray::SetImage(const std::string & fileName, int targetIndex)
{
	int imageWidth;
	int	imageHeight;

	stbi_uc * pixels = LoadFromFile(fileName, imageWidth, imageHeight);

	if (imageWidth != width || imageHeight != height)
		throw std::runtime_error("Mismatching texture size between texture array and assigned texture");

	// TODO: hardcoded pixel size
	VkDeviceSize imageSize = width * height * 4;

	// Copy image to staging buffer region
	memcpy(static_cast< char * >(_stagingData) + imageSize * targetIndex, pixels, imageSize);

	stbi_image_free(pixels);

	// setup staging buffer image to final texture 2D array
	VkBufferImageCopy bufferCopyRegion = {};
	bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	bufferCopyRegion.imageSubresource.mipLevel = 0;
	bufferCopyRegion.imageSubresource.baseArrayLayer = targetIndex;
	bufferCopyRegion.imageSubresource.layerCount = 1;
	bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(width);
	bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(height);
	bufferCopyRegion.imageExtent.depth = 1;
	bufferCopyRegion.bufferOffset = imageSize * targetIndex;

	_bufferCopyRegions.push_back(bufferCopyRegion);
}

void	Texture2DArray::Upload(void)
{
	vkUnmapMemory(device, _stagingBufferMemory);

	const auto & cmd = graphicCommandBufferPool->BeginSingle();

	TransitionImageLayout(image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	vkCmdCopyBufferToImage(
		cmd,
		_stagingBuffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		_bufferCopyRegions.size(),
		_bufferCopyRegions.data()
	);

	TransitionImageLayout(image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

Texture2DArray::Texture2DArray(Texture2DArray const & src)
{
	*this = src;
}

Texture2DArray::~Texture2DArray(void)
{
	vkDestroyBuffer(device, _stagingBuffer, nullptr);
}

Texture2DArray &	Texture2DArray::operator=(Texture2DArray const & src)
{
	Texture::operator=(src);

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture2DArray const & r)
{
	o << "Texture2DArray" << std::endl;
	(void)r;
	return (o);
}
