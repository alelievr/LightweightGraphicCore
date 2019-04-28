#include "Texture3D.hpp"
#include <cmath>

#include "Core/Application.hpp"

using namespace LWGC;

Texture3D::Texture3D(std::size_t width, std::size_t height, std::size_t depth, VkFormat format, int usage)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->format = format;
	this->usage = usage;

	maxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(std::max(width, height), depth)))) + 1;

	AllocateImage(VK_IMAGE_VIEW_TYPE_3D);
}

Texture3D::Texture3D(Texture3D const & src)
{
	*this = src;
}

Texture3D * Texture3D::Create(std::size_t width, std::size_t height, std::size_t depth, VkFormat format, int usage)
{
	return new Texture3D(width, height, depth, format, usage);
}

Texture3D * Texture3D::Create(Texture3D const & src)
{
	return new Texture3D(src);
}

Texture3D &	Texture3D::operator=(Texture3D const & src)
{
	Texture::operator=(src);

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Texture3D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
