#include "Bounds.hpp"

#include <utility>

#include "Utils/Vector.hpp"

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

bool		Bounds::Intersects(const glm::vec3 & origin, const glm::vec3 direction)
{
    float tmin = (_min.x - origin.x) / direction.x;
    float tmax = (_max.x - origin.x) / direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (_min.y - origin.y) / direction.y;
    float tymax = (_max.y - origin.y) / direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (_min.z - origin.z) / direction.z;
    float tzmax = (_max.z - origin.z) / direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}

glm::vec3	Bounds::GetMin(void) const { return _min; }
glm::vec3	Bounds::GetMax(void) const { return _max; }

std::ostream &	LWGC::operator<<(std::ostream & o, Bounds const & b)
{
	o << "(min: " << b.GetMin() << ", max: " << b.GetMax() << ")";
	return o;
}
