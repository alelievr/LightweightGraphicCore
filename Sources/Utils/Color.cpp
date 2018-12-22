#include "Color.hpp"

using namespace LWGC;

Color Color::Black      	= {0, 0, 0};
Color Color::White      	= {1, 1, 1};
Color Color::Gray       	= {0.5, 0.5, 0.5};
Color Color::LighGgray  	= {0.75, 0.75, 0.75};
Color Color::DarkRed    	= {0.5, 0, 0};
Color Color::Red        	= {1, 0, 0};
Color Color::DarkYellow 	= {0.5, 0.5, 0};
Color Color::Yellow     	= {1, 1, 0};
Color Color::DarkGreen  	= {0, 0.5, 0};
Color Color::Green      	= {0, 1, 0};
Color Color::DarkCyan   	= {0, 0.5, 0.5};
Color Color::Cyan       	= {0, 1, 1};
Color Color::DarkBlue   	= {0, 0, 0.5};
Color Color::Blue       	= {0, 0, 1};
Color Color::DarkMagenta	= {0.5, 0, 0.5};
Color Color::Magenta    	= {1, 0, 1};
Color Color::Indigo     	= {0.5, 0, 1};
Color Color::Violet     	= {1, 1, 0.5};
Color Color::DarkBrown  	= {0.5, 0.25, 0};
Color Color::Brown      	= {1, 0.5, 0};

Color::Color(void) : Color(0, 0, 0, 1)
{
}

Color::Color(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a)
{
}

Color::Color(Color const & src)
{
	*this = src;
}

Color::~Color(void)
{
}

Color		Color::HSVToRGB(const float H, const float S, const float V)
{
	(void)H, (void)S, (void)V;
	// TODO:
	return Color();
}

Color		Color::Lerp(const Color & b, const float t)
{
	(void)b, (void)t;
	// TODO:
	return Color();
}

Color		Color::Lerp(const Color & a, const Color & b, const float t)
{
	(void)a, (void)b, (void)t;
	// TODO:
	return Color();
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
