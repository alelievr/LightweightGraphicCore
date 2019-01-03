#include "Texture2D.hpp"
#include <cmath>

using namespace LWGC;

Texture2D::Texture2D(const std::string fileName, VkFormat format, int usage, bool generateMips)
{
	this->format = format;
	// Force transfer flag (as the image comes from the RAM)
    usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	this->usage = usage;

    _pixels = LoadFromFile(fileName, this->width, this->height);

	if (generateMips)
	{
		maxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
	}

    this->depth = 1;
	
	AllocateImage(VK_IMAGE_VIEW_TYPE_2D);

    // UploadImage(_pixels, this->width * this->height * 4);

	UploadImageWithMips(image, format, _pixels, this->width * this->height * 4);
	
	stbi_image_free(_pixels);
}

Texture2D::Texture2D(std::size_t width, std::size_t height, VkFormat format, int usage, bool allocateMips)
{
	this->format = format;
	this->width = width;
	this->height = height;
    this->arraySize = 1;
    this->usage = usage;
	
	maxMipLevel = (allocateMips) ? static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1 : 0;

    AllocateImage(VK_IMAGE_VIEW_TYPE_2D);
}

Texture2D::Texture2D(Texture2D const & src)
{
	*this = src;
}

Texture2D::~Texture2D(void)
{
	vkDestroyImageView(device, view, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, memory, nullptr);
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
	o << "Texture2D" << std::endl;
	(void)r;
	return (o);
}
