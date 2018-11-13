#include "Texture.hpp"

using namespace LWGC;

Texture::Texture(void)
{
	this->width = 0;
	this->height = 0;
	this->autoGenerateMips = false;
}

Texture::Texture(Texture const & src)
{
	*this = src;
}

Texture::~Texture(void)
{
}

Texture &	Texture::operator=(Texture const & src)
{
	if (this != &src)
	{
		this->width = src.width;
		this->height = src.height;
		this->autoGenerateMips = src.autoGenerateMips;
		this->view = src.view;
	}
	return (*this);
}

int				Texture::GetWidth(void) const noexcept { return (this->width); }
int				Texture::GetHeight(void) const noexcept { return (this->height); }
int				Texture::GetDepth(void) const noexcept { return (this->depth); }
VkImageView		Texture::GetView(void) const noexcept { return this->view; }
VkImage			Texture::GetImage(void) const noexcept { return this->image; }
bool			Texture::GetAutoGenerateMips(void) const noexcept { return this->autoGenerateMips; }

std::ostream &	operator<<(std::ostream & o, Texture const & r)
{
	o << "Texture" << std::endl;
	(void)r;
	return (o);
}
