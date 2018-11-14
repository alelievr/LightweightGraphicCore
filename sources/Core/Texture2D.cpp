#include "Texture2D.hpp"

using namespace LWGC;

Texture2D::Texture2D(const std::string fileName, VkFormat format, int usage)
{
	this->format = format;
	// Force transfer flag (as the image comes from the RAM)
    usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	this->usage = usage;

    // stbi_uc * pixels = LoadFromFile(fileName, this->width, this->height);

    // this->depth = 1;
    // AllocateImage();

    // UploadImage(pixels, this->width * this->height * 4, 0, 0);

	int texChannels;
	stbi_uc *pixels = stbi_load(fileName.c_str(), &this->width, &this->height, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = this->width * this->height * 4;

	if (!pixels)
		throw std::runtime_error("failed to load texture image!");

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	VkDevice device = VulkanInstance::Get()->GetDevice();
	Vk::CheckResult(vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data));
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	vkDestroyBuffer(device, stagingBuffer, nullptr);

	stbi_image_free(pixels);

	Vk::CreateImage(this->width, this->height, 1, 1, format, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);

	TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Vk::CopyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height));
	TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	view = Vk::CreateImageView(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
}

Texture2D::Texture2D(std::size_t width, std::size_t height, VkFormat format, int usage)
{
	this->format = format;
	this->width = width;
	this->height = height;
    this->arraySize = 1;
    this->usage = usage;

    AllocateImage();
}

Texture2D::Texture2D(Texture2D const & src)
{
	*this = src;
}

Texture2D::~Texture2D(void)
{
}

// void		Texture2D::Apply(void)
// {
	
// }

// void		Texture2D::SetPixel(int x, int y)
// {
	
// }

// void		Texture2D::GetPixel(int x, int y)
// {
	
// }

Texture2D &	Texture2D::operator=(Texture2D const & src)
{
	Texture::operator=(src);
	
	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture2D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
