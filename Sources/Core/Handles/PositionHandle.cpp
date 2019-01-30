#include "Core/Handles/PositionHandle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Application.hpp"

#include "Utils/Math.hpp"

#define unused __attribute__((unused))

using namespace LWGC;
using namespace Handle;

Position::Position(const glm::vec3 & position) : Gizmo::Position(position), _delta(0, 0, 0), _changed(false)
{
	// _upHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_upHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });
	// _rightHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_rightHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });
	// _forwardHandle.onSelected.AddListener(std::bind(&Position::OnSliderSelected));
	_forwardHandle.onDelta.AddListener([&](auto c, auto d) { OnSliderMoved(c, d); });

	Application::update.AddListener(std::bind(&Position::Update, this));

	_upHandle.onHover.AddListener([&](unused auto c) { upArrow->Hover(); });
	_rightHandle.onHover.AddListener([&](unused auto c) { rightArrow->Hover(); });
	_forwardHandle.onHover.AddListener([&](unused auto c) { forwardArrow->Hover(); });
	_upHandle.onNormal.AddListener([&](unused auto c) { upArrow->Normal(); });
	_rightHandle.onNormal.AddListener([&](unused auto c) { rightArrow->Normal(); });
	_forwardHandle.onNormal.AddListener([&](unused auto c) { forwardArrow->Normal(); });

	_upHandle.BindGameObject(this);
}

Position::~Position(void)
{
}

void		Position::Update(void)
{
	glm::vec3 p0 = transform->GetPosition();

	_upHandle.UpdateWorldPositions(p0, transform->GetUp());
	_rightHandle.UpdateWorldPositions(p0, transform->GetRight());
	_forwardHandle.UpdateWorldPositions(p0, transform->GetForward());
}

// void		Position::OnSliderSelected(HandleControl * control)
// {

// }

void		Position::OnSliderMoved(HandleControl * control, const glm::vec3 delta)
{
	glm::vec3 dir;

	if (control == &_upHandle)
		dir = transform->GetUp();
	else if (control == &_rightHandle)
		dir = transform->GetRight();
	else
		dir = transform->GetForward();

	_changed = true;
	_delta += dir * delta.x; // Slider1D only output in x component
	std::cout << "delta: " << delta.x << std::endl;
	transform->Translate(dir * delta.x * 0.01f);
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
