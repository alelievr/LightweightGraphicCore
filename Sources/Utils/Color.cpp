#include "Color.hpp"

using namespace LWGC;

Color Color::Black      	= {0, 0, 0};
Color Color::White      	= {1, 1, 1};
Color Color::Gray       	= {0.5, 0.5, 0.5};
Color Color::LightGray  	= {0.75, 0.75, 0.75};
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
Color Color::Violet     	= {0.5, 0, 1};
Color Color::DarkBrown  	= {0.5, 0.25, 0};
Color Color::Brown      	= {1, 0.5, 0};

Color::Color(void) : Color(0, 0, 0, 1)
{
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
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
	if (this != &src) {
		this->r = src.r;
		this->g = src.g;
		this->b = src.b;
		this->a = src.a;
	}
	return (*this);
}

Color::operator glm::vec4() const
{
	return glm::vec4(r, g, b, a);
}

std::ostream &	LWGC::operator<<(std::ostream & o, Color const & r)
{
	o << "RGBA(" << r.r << ", " << r.g << ", " << r.b << ", " << r.a << ")";
	(void)r;
	return (o);
}
