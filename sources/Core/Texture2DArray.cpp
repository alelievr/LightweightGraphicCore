#include "Texture2DArray.hpp"

using namespace LWGC;

#include STB_INCLUDE_IMAGE

Texture2DArray::Texture2DArray(int width, int height, int arraySize, VkFormat format, int usage)
{
	this->format = format;
	this->width = width;
	this->height = height;
	this->arraySize = arraySize;
	this->usage = usage;

	AllocateImage();

	// TODO: hardcoded pixel size
	VkDeviceSize imageSize = width * height * 4 * arraySize;
	Vk::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingBufferMemory);
	VkDevice device = VulkanInstance::Get()->GetDevice();
	vkMapMemory(device, _stagingBufferMemory, 0, imageSize, 0, &_stagingData);
}

void	Texture2DArray::SetImage(const std::string & fileName, int targetIndex)
{
	int imageWidth;
	int	imageHeight;

	stbi_uc * pixels = LoadFromFile(fileName, imageWidth, imageHeight);

	if (imageWidth != width || imageHeight != height)
		throw new std::runtime_error("Mismatching texture size between texture array and assigned texture");
		
	VkDeviceSize imageSize = width * height * 4;

	// Copy image to staging buffer region
}

void	Texture2DArray::Upload(void)
{

}

Texture2DArray::Texture2DArray(Texture2DArray const & src)
{
	*this = src;
}

Texture2DArray::~Texture2DArray(void)
{
	const auto & device = VulkanInstance::Get()->GetDevice();
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
