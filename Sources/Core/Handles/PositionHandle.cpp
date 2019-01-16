#include "Core/Handles/PositionHandle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Application.hpp"

#include "Utils/Math.hpp"

using namespace LWGC;
using namespace Handles;

Position::Position(const glm::vec3 & position) : Gizmo::Position(position), _delta(0, 0, 0), _changed(false)
{
	// _upHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_upHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });
	// _rightHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_rightHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });
	// _forwardHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_forwardHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });
}

Position::~Position(void)
{
}

// void		Position::OnSliderSelected(IHandleControl * control)
// {

// }

void		Position::OnSliderMoved(IHandleControl * control, const glm::vec3 delta)
{
	_changed = true;
	_delta += delta;
}

bool		Position::HasChanged(void)
{
	return _changed;
}

glm::vec3	Position::GetDelta(void)
{
	glm::vec3 delta = _delta;
	_delta = glm::vec3(0, 0, 0);
	_changed = false;
	return delta;
}

std::ostream &	operator<<(std::ostream & o, Position const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
