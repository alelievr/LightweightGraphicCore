#include "Core/Handles/HandleUtils.hpp"

#include "Utils/Vector.hpp"

using namespace LWGC;
using namespace Handle;

glm::vec3 HandleUtils::ProjectPointLine(const glm::vec3 & segment0, const glm::vec3 & segment1, const glm::vec3 & point)
{
	glm::vec3 relativePoint = point - segment0;
	glm::vec3 lineDirection = segment1 - segment0;
	float length = glm::length(lineDirection);
	glm::vec3 normalizedLineDirection = lineDirection;
	if (length > .000001f)
		normalizedLineDirection /= length;

	float dot = glm::dot(normalizedLineDirection, relativePoint);
	dot = fminf(fmaxf(dot, 0.0F), length);

	return segment0 + normalizedLineDirection * dot;
}

float	HandleUtils::DistanceToSegment(const glm::vec3 & segment0, const glm::vec3 & segment1, const glm::vec3 & point)
{
	return glm::length(ProjectPointLine(segment0, segment1, point) - point);
}
