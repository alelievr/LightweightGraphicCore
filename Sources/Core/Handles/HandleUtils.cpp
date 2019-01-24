#include "Core/Handles/HandleUtils.hpp"

#include "Utils/Vector.hpp"

using namespace LWGC;
using namespace Handle;

float	HandleUtils::DistanceToSegment(const glm::vec2 & segment0, const glm::vec2 & segment1, const glm::vec2 & point)
{
	// FIXME: totally broken, behave like min(dist(p0, p), dist(p1, p));
	// segment - point distance:
	const float l2 = glm::length(segment0 - segment1);
	if (l2 == 0.0)
		return glm::distance(point, segment0);
	const float t = fmaxf(0, fminf(1, glm::dot(point - segment0, segment1 - segment0) / l2));
	const glm::vec2 projection = segment0 + t * (segment1 - segment0);
	std::cout << "point: " << point << std::endl;
	std::cout << "dist: " << glm::distance(point, projection) << std::endl;
	return glm::distance(point, projection);
}
