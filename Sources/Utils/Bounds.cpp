#include "Bounds.hpp"

using namespace LWGC;

Bounds::Bounds(void)
{
	_min = glm::vec3(0.f);
	_max = glm::vec3(0.f);
}

Bounds::Bounds(const glm::vec3 & min, const glm::vec3 & max)
{
	_min = min;
	_max = max;
}

Bounds::Bounds(const Bounds & b)
{
	*this = b;
}

Bounds &	Bounds::operator=(const Bounds & b)
{
	if (this != &b)
	{
		_min = b.GetMin();
		_max = b.GetMax();
	}

	return *this;
}

void	Bounds::Encapsulate(const glm::vec3 & p)
{
	_min = glm::min(_min, p);
	_max = glm::max(_max, p);
}

glm::vec3	Bounds::GetMin(void) const { return _min; }
glm::vec3	Bounds::GetMax(void) const { return _max; }
