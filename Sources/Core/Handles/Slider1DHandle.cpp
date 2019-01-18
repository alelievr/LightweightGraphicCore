#include "Core/Handles/Slider1DHandle.hpp"

#include "Core/EventSystem.hpp"

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
}

float		Slider1D::UpdateDistance(Camera * cam)
{
	glm::vec2 screenPoint0 = cam->WorldToScreenPoint(_worldPoint0);
	glm::vec2 screenPoint1 = cam->WorldToScreenPoint(_worldPoint1);

	glm::vec2 mousePos = EventSystem::Get()->GetCursorPosition() / cam->GetViewportSize() * 2.0f - 1.0f;

	// segment - point distance:
	const float l2 = glm::length(screenPoint0 - screenPoint1);
	if (l2 == 0.0)
		return glm::distance(mousePos, screenPoint0);
	const float t = fmaxf(0, fminf(1, glm::dot(mousePos - screenPoint0, screenPoint1 - screenPoint0) / l2));
	const glm::vec2 projection = screenPoint0 + t * (screenPoint1 - screenPoint0);
	float dist = glm::distance(mousePos, projection);
	std::cout << "dist: " << dist << std::endl;
	return dist;
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
