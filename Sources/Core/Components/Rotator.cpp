#include "Rotator.hpp"

#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

void			Rotator::Update(void) noexcept
{
	static float r = 0;

	r++;

	transform->RotateAxis(1 * Math::DegToRad, glm::vec3(1, 0, 0));
}

uint32_t		Rotator::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::Rotator);
}

std::ostream &	operator<<(std::ostream & o, Rotator const & r)
{
	o << "Rotator" << std::endl;
	(void)r;
	return (o);
}
