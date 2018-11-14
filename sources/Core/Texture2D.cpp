#include "Texture2D.hpp"

using namespace LWGC;

Texture2D::Texture2D(const std::string fileName, VkFormat format, int usage)
{
	this->format = format;
	// Force transfer flag (as the image comes from the RAM)
    usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	this->usage = usage;

    stbi_uc * pixels = LoadFromFile(fileName, this->width, this->height);

    this->depth = 1;
    AllocateImage();

    UploadImage(pixels, this->width * this->height * 4, 0, 0);
	
	stbi_image_free(pixels);
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
	const auto & device = VulkanInstance::Get()->GetDevice();
	
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
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
