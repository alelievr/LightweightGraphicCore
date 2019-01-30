#include "Activator.hpp"

#include "Core/Application.hpp"
#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

void			Activator::Initialize(void) noexcept
{
	Application::update.AddListener(std::bind(&Activator::ForceUpdate, this));
}

void			Activator::ForceUpdate(void) noexcept
{
	static int r = 0;

	if (r % 60 == 0)
	{
		gameObject->SetActive(!gameObject->IsActive());
	}

	r++;
}

uint32_t		Activator::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::Activator);
}

std::ostream &	operator<<(std::ostream & o, Activator const & r)
{
	o << "Activator" << std::endl;
	(void)r;
	return (o);
}
