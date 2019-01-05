#include "Ray.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

using namespace LWGC;
using namespace Gizmo;

Ray::Ray(const glm::vec3 origin, const glm::vec3 direction, const Color & c) : Line(origin, origin + direction, c), _origin(origin)
{
}

Ray::~Ray(void)
{
}

// TODO: update mesh and upload to device
void	Ray::SetOrigin(const glm::vec3 origin) { SetPoint0(origin); _origin = origin; }
void	Ray::SetDirection(const glm::vec3 direction) { SetPoint1(_origin + direction); }

std::ostream &	operator<<(std::ostream & o, Ray const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
