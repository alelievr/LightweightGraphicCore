#include "Texture.hpp"

#include "TextureFilterMode.hpp"
#include "TextureWrapMode.hpp"

using namespace LWGE;

Texture::Texture(void)
{
	std::cout << "Default constructor of Texture called" << std::endl;
	this->_width = 0;
	this->_height = 0;
	this->_filterMode = TextureFilterMode::Bilinear;
	this->_wrapMode = TextureWrapMode::Repeat;
}

Texture::Texture(Texture const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Texture::~Texture(void)
{
	std::cout << "Destructor of Texture called" << std::endl;
}


Texture &	Texture::operator=(Texture const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_width = src.getWidth();
		this->_height = src.getHeight();
		this->_filterMode = src.getFilterMode();
		this->_wrapMode = src.getWrapMode();
	}
	return (*this);
}

std::size_t		Texture::GetWidth(void) const { return (this->_width); }
void		Texture::SetWidth(std::size_t tmp) { this->_width = tmp; }

std::size_t		Texture::GetHeight(void) const { return (this->_height); }
void		Texture::SetHeight(std::size_t tmp) { this->_height = tmp; }

TextureFilterMode		Texture::GetFilterMode(void) const { return (this->_filterMode); }
void		Texture::SetFilterMode(TextureFilterMode tmp) { this->_filterMode = tmp; }

TextureWrapMode		Texture::GetWrapMode(void) const { return (this->_wrapMode); }
void		Texture::SetWrapMode(TextureWrapMode tmp) { this->_wrapMode = tmp; }

std::ostream &	operator<<(std::ostream & o, Texture const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
