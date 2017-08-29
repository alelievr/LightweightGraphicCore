#include "Image.hpp"

Image::Image(void)
{
	std::cout << "Default constructor of Image called" << std::endl;
	this->_file = "";
	this->_id = ;
}

Image::Image(Image const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Image::~Image(void)
{
	std::cout << "Destructor of Image called" << std::endl;
}


Image &	Image::operator=(Image const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_file = src.getFile();
		this->_id = src.getId();
	}
	return (*this);
}

std::string		Image::getFile(void) const { return (this->_file); }
void		Image::setFile(std::string tmp) { this->_file = tmp; }

GLuint		Image::getId(void) const { return (this->_id); }
void		Image::setId(GLuint tmp) { this->_id = tmp; }

std::ostream &	operator<<(std::ostream & o, Image const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
