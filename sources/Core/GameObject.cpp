#include "GameObject.hpp"

using namespace LWGC;

GameObject::GameObject(void)
{
	std::cout << "Default constructor of GameObject called" << std::endl;
	this->_name = "GameObject";
	this->_flags = 0;
}

GameObject::GameObject(GameObject const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

GameObject::~GameObject(void)
{
	std::cout << "Destructor of GameObject called" << std::endl;
}


GameObject &	GameObject::operator=(GameObject const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_trasform = src.GetTransform();
		this->_name = src.GetName();
		this->_flags = src.GetFlags();
	}
	return (*this);
}

Transform		GameObject::GetTransform(void) const { return (this->_trasform); }
void			GameObject::SetTransform(Transform tmp) { this->_trasform = tmp; }

IComponent *	AddComponent(IComponent * component) noexcept
{
	_components.insert(component);
	// TODO: update internal rendering lists if the components has graphic capabilities
}

void			RemoveComponent(IComponent * component) noexcept
{
	_components.erase(component);
}

std::ostream &	operator<<(std::ostream & o, GameObject const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
