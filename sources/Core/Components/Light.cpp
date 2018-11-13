#include "Light.hpp"


Light::Light(void)
{
	std::cout << "Default constructor of Light called" << std::endl;
}

Light::Light(Light const & src)
{
	*this = src;
	std::cout << "Copy constructor of Light called" << std::endl;
}

Light::~Light(void)
{
	std::cout << "Destructor of Light called" << std::endl;
}

virtual void		Light::OnAdded(const GameObject & go) noexcept
{
	
}

virtual void		Light::OnRemoved(const GameObject & go) noexcept
{
	
}


Light &	Light::operator=(Light const & src)
{
	std::cout << "Assignment operator of Light called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, Light const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
