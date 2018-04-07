#include "Random.hpp"

using namespace LWGE;

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

float		Random::GetFloat(void)
{
	
}

int		Random::GetInt(void)
{
	
}

glm::vec2		Random::OnCircle(void)
{
	
}

glm::vec2		Random::InsideCircle(void)
{
	
}

glm::vec3		Random::OnSphere(void)
{
	
}

glm::vec3		Random::InsideSphere(void)
{
	
}

glm::quat		Random::GetRotation(void)
{
	
}

Color		Random::GetColor(void)
{
	
}

float		Random::Range(const float a, const float b)
{
	
}

int		Random::Range(const int a, const int b)
{
	
}

void		Random::SetSeed(const long seed)
{
	
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
