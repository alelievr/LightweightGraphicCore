#include "GameObject.hpp"

using namespace LWGC;

GameObject::GameObject(void)
{
	std::cout << "Default constructor of GameObject called" << std::endl;
	this->_name = "GameObject";
	this->_flags = 0;
}

GameObject::GameObject(Component * component)
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

Component *	GameObject::AddComponent(Component * component) noexcept
{
	_components.insert(component);
	component->OnAdded(*this);

	return component;
}

void			GameObject::RemoveComponent(Component * component) noexcept
{
	component->OnRemoved(*this);
	_components.erase(component);
}

Hierarchy *		GameObject::GetHierarchy(void) const noexcept
{
	return _hierarchy;
}

Transform		GameObject::GetTransform(void) const { return (this->_transform); }
void			GameObject::SetTransform(Transform tmp) { this->_transform = tmp; }

void			GameObject::SetActive(bool active)
{
	if (active == _active)
		return ;
	
	_active = active;

	for (const auto & comp : _components)
		comp->UpdateGameObjectActive();
}

bool			GameObject::IsActive(void) const { return _active; }

void			GameObject::SetHierarchy(Hierarchy * hierarchy)
{
	_hierarchy = hierarchy;
}

std::ostream &	operator<<(std::ostream & o, GameObject const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
