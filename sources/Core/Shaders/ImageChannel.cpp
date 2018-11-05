#include "ImageChannel.hpp"

using namespace LWGC;

ImageChannel::ImageChannel(void)
{
	std::cout << "Default constructor of ImageChannel called" << std::endl;
	this->_imageId = 0;
	this->_type = ChannelType::Image;
	this->_imagePath = nullptr;
}

ImageChannel::ImageChannel(ImageChannel const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

ImageChannel::~ImageChannel(void)
{
	std::cout << "Destructor of ImageChannel called" << std::endl;
}

int			ImageChannel::Bind(void)
{

}

ChannelType		ImageChannel::GetType(void)
{
	
}


ImageChannel &	ImageChannel::operator=(ImageChannel const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_imageId = src.GetImageId();
		this->_type = src.GetType();
		this->_imagePath = src.GetImagePath();
	}
	return (*this);
}

int		ImageChannel::GetImageId(void) const { return (this->_imageId); }
void		ImageChannel::SetImageId(int tmp) { this->_imageId = tmp; }

ChannelType		ImageChannel::GetType(void) const { return (this->_type); }
void		ImageChannel::SetType(ChannelType tmp) { this->_type = tmp; }

std::string		ImageChannel::GetImagePath(void) const { return (this->_imagePath); }
void		ImageChannel::SetImagePath(std::string tmp) { this->_imagePath = tmp; }

std::ostream &	operator<<(std::ostream & o, ImageChannel const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
