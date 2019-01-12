#include "PositionHandle.hpp"

#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Application.hpp"

#include "Utils/Math.hpp"

using namespace LWGC;
using namespace Gizmo;

PositionHandle::PositionHandle(const glm::vec3 & position) : Position(position)
{
	// TODO: Add 3 line handles to control the 3 arrows
}

PositionHandle::~PositionHandle(void)
{
}

bool		PositionHandle::HasChanged(void)
{
	return false;
}

glm::vec3	PositionHandle::GetDelta(void)
{
	// TODO
	return glm::vec3(0, 0, 0);
}

void		PositionHandle::Select(void)
{
	// TODO: movement
}

void		PositionHandle::UnSelect(void)
{
}


std::ostream &	operator<<(std::ostream & o, PositionHandle const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
