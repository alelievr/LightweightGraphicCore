#include "Random.hpp"


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

static float		Random::GetFloat(void)
{
	
}

static int		Random::GetInt(void)
{
	
}

static glm::vec2		Random::OnCircle(void)
{
	
}

static glm::vec2		Random::InsideCircle
{
	
}

static glm::vec3		Random::OnSphere(void)
{
	
}

static glm::vec3		Random::InsideSphere(void)
{
	
}

static glm::quat		Random::GetRotation(void)
{
	
}

static Color		Random::GetColor(void)
{
	
}

static float		Random::Range(const float a, const float b)
{
	
}

static int		Random::Range(const int a, const int b)
{
	
}

static void		Random::SetSeed(const long seed)
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
