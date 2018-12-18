#include "Vector.hpp"

using namespace LWGC;

#include "IncludeDeps.hpp"
#include GLM_INCLUDE_STRING_CAST

Vector::~Vector(void)
{
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::vec2 & r)
{
	o << glm::to_string(r);
	return (o);
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::vec3 & r)
{
	o << glm::to_string(r);
	return (o);
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::vec4 & r)
{
	o << glm::to_string(r);
	return (o);
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::mat4 & r)
{
	o << glm::to_string(r);
	return (o);
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::mat3 & r)
{
	o << glm::to_string(r);
	return (o);
}

std::ostream &	LWGC::operator<<(std::ostream & o, const glm::quat & r)
{
	o << glm::to_string(r);
	return (o);
}
