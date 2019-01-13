#include "Random.hpp"

#include <climits>
#include <cmath>

using namespace LWGC;

Random::Random(void)
{
	std::cout << "Default constructor of Random called" << std::endl;
}

Random::Random(Random const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Random::~Random(void)
{
	std::cout << "Destructor of Random called" << std::endl;
}

float			Random::GetValue(void)
{
	return drand48();
}

glm::vec2		Random::OnCircle(void)
{
	float p = drand48() * 2 * M_PI;

	return glm::vec2(cos(p), sin(p));
}

glm::vec2		Random::InsideCircle(void)
{
	return OnCircle() * (float)drand48();
}

glm::vec3		Random::OnSphere(void)
{
	return glm::normalize(InsideSphere());
}

glm::vec3		Random::InsideSphere(void)
{
	glm::vec3 p;

	do
	{
		p = glm::vec3(drand48(), drand48(), drand48()) * 2.0f - 1.0f;
	} while (glm::dot(p, p) >= 1);

	return p;
}

glm::quat		Random::GetRotation(void)
{
	return glm::normalize(glm::quat(GetValue(), GetValue(), GetValue(), GetValue()));
}

glm::vec3		Random::GetPosition(const glm::vec3 & min, const glm::vec3 & max)
{
	return glm::vec3(Range(min.x, max.x), Range(min.y, max.y), Range(min.z, max.z));
}

Color			Random::GetColor(void)
{
	return Color(GetValue(), GetValue(), GetValue(), 1);
}

float			Random::Range(const float a, const float b)
{
	return drand48() * (b - a) + a;
}

int				Random::Range(const int a, const int b)
{
	return mrand48() % (b - a) + a;
}

void			Random::SetSeed(const long seed)
{
	srand48(seed);
}


Random &	Random::operator=(Random const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Random const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
