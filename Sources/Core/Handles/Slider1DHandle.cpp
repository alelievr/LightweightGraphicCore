#include "Core/Handles/Slider1DHandle.hpp"

#include "Core/EventSystem.hpp"
#include "Core/Handles/HandleUtils.hpp"

using namespace LWGC;
using namespace Handle;

Slider1D::Slider1D(void)
{
}

void		Slider1D::UpdateWorldPositions(const glm::vec3 & p0, const glm::vec3 & p1)
{
	_worldPoint0 = p0;
	_worldPoint1 = p1;
}

void		Slider1D::UpdateSelected(void)
{
	// TODO: use the delta of the projected mouse point on the line instead of mouse delta
	std::cout << "TODO: UpdateSelected of Slider1DHandle" << std::endl;
	glm::vec3 delta = glm::vec3(EventSystem::Get()->delta.y, 0, 0);
	onDelta.Invoke(this, delta);
}

float		Slider1D::UpdateDistance(Camera * cam)
{
	glm::vec3 screenPoint0 = cam->WorldToScreenPoint(_worldPoint0);
	glm::vec3 screenPoint1 = cam->WorldToScreenPoint(_worldPoint1);

	glm::vec2 mousePos = EventSystem::Get()->GetNormalizedCursorPosition();

	return HandleUtils::DistanceToSegment(screenPoint0, screenPoint1, glm::vec3(mousePos, 0));
}

Slider1D::~Slider1D(void)
{
}

std::ostream &	operator<<(std::ostream & o, Slider1D const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
