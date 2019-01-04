#include "Line.hpp"

using namespace LWGC;
using namespace Gizmo;

Line::Line(const glm::vec3 p0, const glm::vec3 p1)
{
}

Line::~Line(void)
{
}

std::ostream &	operator<<(std::ostream & o, Line const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
