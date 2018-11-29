#include "Light.hpp"

using namespace LWGC;

Light::Light(void)
{
	std::cout << "Default constructor of Light called" << std::endl;
}

Light::~Light(void)
{
	std::cout << "Destructor of Light called" << std::endl;
}

void		Light::OnAdded(const GameObject & go) noexcept
{
	Component::OnAdded(go);
}

void		Light::OnRemoved(const GameObject & go) noexcept
{
	Component::OnRemoved(go);
}

std::ostream &	operator<<(std::ostream & o, Light const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
