#include "Texture2DAtlas.hpp"
#include "Utils/Vector.hpp"
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
	// Force transfer flag (as the image comes from the RAM)
	this->usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	this->_atlasSize = glm::vec4(w, h, 1.0f / glm::vec2(w, h));

	_maxMipLevel = (allocateMips) ? static_cast<uint32_t>(std::floor(std::log2(std::max(w, h)))) + 1 : 0;

	AllocateImage(VK_IMAGE_VIEW_TYPE_2D);
	Vk::CreateBuffer(
		sizeof(_atlasSize),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		this->_atlasSizeBuffer,
		this->_atlasSizeMemory
	);
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
	glm::ivec3 imgSize = glm::ivec3(width ,height, 1);
	glm::ivec3 offset = glm::ivec3(rect.GetMinX(), rect.GetMinY(), 0);
	glm::vec4 sizeoffset = glm::vec4(
		glm::vec2(imgSize.x, imgSize.y) / glm::vec2(this->width, this->height), 
		glm::vec2(offset.x, offset.y) / glm::vec2(this->width, this->height)
	);
	this->_sizeOffsets.push_back(sizeoffset);
	UploadImage(_pixels, width * height * 4, imgSize, offset);

	stbi_image_free(_pixels);
	return rect;
}

void	Texture2DAtlas::UploadAtlasDatas(void)
{
	Vk::CreateBuffer(
		GetSizeOffsetBufferSize(),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		this->_sizeOffsetsBuffer,
		this->_sizeOffsetsMemory
	);
	Vk::UploadToMemory(_sizeOffsetsMemory, _sizeOffsets.data(), GetSizeOffsetBufferSize());
	Vk::UploadToMemory(_atlasSizeMemory, &_atlasSize, GetAtlasSizeBufferSize());
	std::cout << _atlasSize << std::endl;
}

void	Texture2DAtlas::Clear(void)
{
	_nodetree.Clear();
}

VkBuffer	Texture2DAtlas::GetSizeOffsetBuffer() { return _sizeOffsetsBuffer; }
VkBuffer	Texture2DAtlas::GetAtlasSizeBuffer() { return _atlasSizeBuffer; }
size_t		Texture2DAtlas::GetSizeOffsetBufferSize() { return _sizeOffsets.size() * sizeof(glm::vec4); }
size_t		Texture2DAtlas::GetAtlasSizeBufferSize() { return sizeof(glm::vec4); }

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
