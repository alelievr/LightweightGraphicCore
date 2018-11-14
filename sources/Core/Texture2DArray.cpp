#include "Texture2DArray.hpp"

using namespace LWGC;

#include STB_INCLUDE_IMAGE

Texture2DArray::Texture2DArray(int width, int height, int arraySize, VkFormat format, int usage) : _arraySize(arraySize)
{
	this->format = format;
	this->width = width;
	this->height = height;
	this->arraySize = 1;
	this->usage = usage;

	AllocateImage();
}

void	Texture2DArray::SetImage(const std::string & fileName, int targetIndex)
{
	int imageWidth;
	int	imageHeight;

	stbi_uc * pixels = LoadFromFile(fileName, imageWidth, imageHeight);

	if (imageWidth != width || imageHeight != height)
		throw new std::runtime_error("Mismatching texture size between texture array and assigned texture");

	// TODO: multiple image format support
	UploadImage(pixels, width * height * 4, targetIndex, 0);
}

Texture2DArray::Texture2DArray(Texture2DArray const & src)
{
	*this = src;
}

Texture2DArray::~Texture2DArray(void)
{
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
