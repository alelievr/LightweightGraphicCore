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
	glm::vec3 delta = glm::vec3(EventSystem::Get()->delta.y, 0, 0);
	onDelta.Invoke(this, delta);
	std::cout << "update selected !\n";
}

float		Slider1D::UpdateDistance(Camera * cam)
{
	glm::vec2 screenPoint0 = cam->WorldToScreenPoint(_worldPoint0);
	glm::vec2 screenPoint1 = cam->WorldToScreenPoint(_worldPoint1);

	std::cout << "ScreenPos0: " << screenPoint0.x << ", " << screenPoint0.y << std::endl;
	std::cout << "ScreenPos1: " << screenPoint1.x << ", " << screenPoint1.y << std::endl;

	std::cout << "Cursor pos: " << EventSystem::Get()->GetNormalizedCursorPosition().y << std::endl;
	glm::vec2 mousePos = EventSystem::Get()->GetNormalizedCursorPosition();

	return HandleUtils::DistanceToSegment(screenPoint0, screenPoint1, mousePos);
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
