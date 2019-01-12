#include "Position.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Hierarchy.hpp"

#include "Utils/Math.hpp"

using namespace LWGC;
using namespace Gizmo;

Position::Position(const glm::vec3 & position)
{
	transform->SetPosition(position);

	upArrow = new Arrow(1, 100, false, Color::Green);
	rightArrow = new Arrow(1, 100, false, Color::Red);
	forwardArrow = new Arrow(1, 100, false, Color::Blue);
}

void	Position::Initialize(void) noexcept
{
	GameObject::Initialize();

	hierarchy->AddGameObject(upArrow);
	hierarchy->AddGameObject(rightArrow);
	hierarchy->AddGameObject(forwardArrow);

	upArrow->GetTransform()->SetRotation(glm::angleAxis(0.0f * Math::DegToRad, glm::vec3(0, 1, 0)));
	rightArrow->GetTransform()->SetRotation(glm::angleAxis(90.0f * Math::DegToRad, glm::vec3(0, 0, 1)));
	forwardArrow->GetTransform()->SetRotation(glm::angleAxis(90.0f * Math::DegToRad, glm::vec3(1, 0, 0)));

	// parent all these transforms
	upArrow->GetTransform()->SetParent(transform);
	rightArrow->GetTransform()->SetParent(transform);
	forwardArrow->GetTransform()->SetParent(transform);
}

Position::~Position(void)
{
}

std::ostream &	operator<<(std::ostream & o, Position const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
