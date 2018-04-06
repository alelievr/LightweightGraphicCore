#include "Color.hpp"


Color::Color(void)
{
	std::cout << "Default constructor of Color called" << std::endl;
	this->_r = 0;
	this->_g = 0;
	this->_b = 0;
	this->_a = 0;
}

Color::Color(Color const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Color::~Color(void)
{
	std::cout << "Destructor of Color called" << std::endl;
}

static Color		Color::HSVToRGB(const float H, const float S, const float V) const
{
	
}

Color		Color::Lerp(const Color & b, const float t)
{
	
}

static Color		Color::Lerp(const Color & a, const Color & b, const float t)
{
	
}


Color &	Color::operator=(Color const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_r = src.GetR();
		this->_g = src.GetG();
		this->_b = src.GetB();
		this->_a = src.GetA();
	}
	return (*this);
}

float		Color::GetR(void) const { return (this->_r); }
void		Color::SetR(float tmp) { this->_r = tmp; }

float		Color::GetG(void) const { return (this->_g); }
void		Color::SetG(float tmp) { this->_g = tmp; }

float		Color::GetB(void) const { return (this->_b); }
void		Color::SetB(float tmp) { this->_b = tmp; }

float		Color::GetA(void) const { return (this->_a); }
void		Color::SetA(float tmp) { this->_a = tmp; }

std::ostream &	operator<<(std::ostream & o, Color const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
