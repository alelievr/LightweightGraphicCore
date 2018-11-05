#include "GameObject.hpp"

using namespace LWGC;

GameObject::GameObject(void)
{
	std::cout << "Default constructor of GameObject called" << std::endl;
	this->_name = "GameObject";
	this->_flags = 0;
}

GameObject::GameObject(IComponent * component)
{
	AddComponent(component);
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
		this->_transform = src.GetTransform();
		this->_name = src.GetName();
		this->_flags = src.GetFlags();
	}
	return (*this);
}

IComponent *	GameObject::AddComponent(IComponent * component) noexcept
{
	_components.insert(component);
	component->OnAdded(*this);
}

void			GameObject::RemoveComponent(IComponent * component) noexcept
{
	component->OnRemoved(*this);
	_components.erase(component);
}

Transform		GameObject::GetTransform(void) const { return (this->_transform); }
void			GameObject::SetTransform(Transform tmp) { this->_transform = tmp; }

void			GameObject::SetActive(bool active) { _active = active; }
bool			GameObject::IsActive(void) const { return _active; }

std::ostream &	operator<<(std::ostream & o, GameObject const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
