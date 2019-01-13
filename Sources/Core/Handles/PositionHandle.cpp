#include "Core/Handles/PositionHandle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Application.hpp"

#include "Utils/Math.hpp"

using namespace LWGC;
using namespace Handles;

Position::Position(const glm::vec3 & position) : Gizmo::Position(position)
{
	// TODO: Add 3 line handles to control the 3 arrows
}

Position::~Position(void)
{
}

bool		Position::HasChanged(void)
{
	return false;
}

glm::vec3	Position::GetDelta(void)
{
	// TODO
	return glm::vec3(0.1f, 0, 0);
}

void		Position::Select(void)
{
	std::cout << "SELECT !" << std::endl;;
}

void		Position::UnSelect(void)
{
}


std::ostream &	operator<<(std::ostream & o, Position const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
