#include "Movator.hpp"

#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

void			Movator::Update(void) noexcept
{
	static float r = 0;

	r++;

	transform->Translate(glm::vec3(1 * 0.01, 0, 0));
}

uint32_t		Movator::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::Movator);
}

std::ostream &	operator<<(std::ostream & o, Movator const & r)
{
	o << "Movator" << std::endl;
	(void)r;
	return (o);
}
