#include "Template.hpp"

using namespace LWGC;

Template::Template(void)
{
}

Template::~Template(void)
{
}

void			Template::OnAdded(GameObject & go) noexcept
{
	Component::OnAdded(go);
}

void			Template::OnRemoved(const GameObject & go) noexcept
{
	Component::OnRemoved(go);
}

void			Template::OnEnable(void) noexcept
{
	Component::OnEnable();
}

void			Template::OnDisable(void) noexcept
{
	Component::OnDisable();
}

void			Template::Initialize(void) noexcept
{
	Component::Initialize();
}

uint32_t		Template::GetType(void) const noexcept
{
	return type;
}

std::ostream &	operator<<(std::ostream & o, Template const & r)
{
	o << "Template" << std::endl;
	(void)r;
	return (o);
}
