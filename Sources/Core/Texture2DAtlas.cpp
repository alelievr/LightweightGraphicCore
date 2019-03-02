#include "Texture2DAtlas.hpp"
#include <cmath>
#include <vector>

using namespace LWGC;

// check in is power of 2
Texture2DAtlas::Texture2DAtlas(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips)
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

// Creat

Texture2DAtlas::~Texture2DAtlas(void)
{
	std::cout << "Destructor of Texture2DAtlas called" << std::endl;
}

void *Texture2DAtlas::Fit(std::string fileName)
{
	int width, height;

	_pixels = LoadFromFile(fileName, width, height);
	
	// auto node = FindNode(width, height);
	// if (node != NULL)
	// {
	// 	//save texture in node;
	// 	return node;
	// }
	stbi_image_free(_pixels);
}

// void *Texture2DAtlas::Findnode(uint32_t w, uint32_t h)
// {
// 	// if (root.used)
// 	// 	return this.findNode(root.right, w, h) || this.findNode(root.down, w, h);
// 	// else 
// 	if ((width <= w) && (height <= h))
// 		return NULL; //mem_ptr;
// 	else
// 		return NULL;
// }

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
