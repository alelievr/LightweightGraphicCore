#include "GameObject.hpp"

using namespace LWGC;

GameObject::GameObject(void) : _active(false), _initialized(false)
{
	std::cout << "Default constructor of GameObject called" << std::endl;
	this->_name = "GameObject";
	this->_flags = 0;
}

GameObject::GameObject(Component * component) : GameObject()
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
		this->_initialized = src._initialized;
		this->_transform = src.GetTransform();
		this->_name = src.GetName();
		this->_flags = src.GetFlags();
	}
	return (*this);
}

void		GameObject::Initialize(void) noexcept
{
	_initialized = true;
	for (const auto & component : _components)
	{
		component->Initialize();
		component->OnAdded(*this);
	}

	SetActive(true);
}

Component *		GameObject::AddComponent(Component * component) noexcept
{
	if (_initialized)
	{
		component->Initialize();
		component->OnAdded(*this);
	}
	_components.insert(component);

	return component;
}

Component *		GameObject::GetComponent(void) noexcept
{
	// TODO: hardcoded single-component object
	return *_components.begin();
}

void			GameObject::RemoveComponent(Component * component) noexcept
{
	component->OnRemoved(*this);
	_components.erase(component);
}

void			GameObject::SetHierarchy(Hierarchy * hierarchy) { _hierarchy = hierarchy; }
Hierarchy *		GameObject::GetHierarchy(void) const noexcept { return _hierarchy; }

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

std::ostream &	operator<<(std::ostream & o, GameObject const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
