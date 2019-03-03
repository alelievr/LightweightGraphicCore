#include "Texture2DAtlas.hpp"
#include <cmath>
#include <vector>

using namespace LWGC;

// check in is power of 2
Texture2DAtlas::Texture2DAtlas(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips) : _nodetree(w, h)
{
	if ((((w * h) != 0) && ( (w * h) &  ((w * h) - 1)) == 1) )
		throw std::runtime_error("Texture2DAtlas needs to be power of 2");

	this->format = format;
	this->width = w;
	this->height = h;
	this->arraySize = 1;
	this->usage = usage;

	_maxMipLevel = (allocateMips) ? static_cast<uint32_t>(std::floor(std::log2(std::max(w, h)))) + 1 : 0;

	AllocateImage(VK_IMAGE_VIEW_TYPE_2D);
}

Texture2DAtlas::~Texture2DAtlas(void)
{
	std::cout << "Destructor of Texture2DAtlas called" << std::endl;
}

Texture2DAtlas *Texture2DAtlas::Create(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips)
{
	return new Texture2DAtlas(w, h, format, usage, allocateMips);
}

Rect Texture2DAtlas::Fit(std::string fileName)
{
	int width, height;

	_pixels = LoadFromFile(fileName, width, height);
	Rect rect = _nodetree.Allocate(width, height);
	UploadImage(_pixels, width * height * 4, rect.GetMinX(), rect.GetMinY());

	stbi_image_free(_pixels);
	return rect;
}

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
