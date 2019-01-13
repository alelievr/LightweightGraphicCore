#include "GameObject.hpp"

#include "Core/Hierarchy.hpp"

using namespace LWGC;

GameObject::GameObject(void) : _active(false), _initialized(false)
{
	this->transform = new Transform();
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
}

GameObject::~GameObject(void)
{
	delete transform;
}

GameObject &	GameObject::operator=(GameObject const & src)
{
	if (this != &src)
	{
		this->_initialized = src._initialized;
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

void			GameObject::SetHierarchy(Hierarchy * hierarchy) { this->hierarchy = hierarchy; }
Hierarchy *		GameObject::GetHierarchy(void) const noexcept { return hierarchy; }

Transform *		GameObject::GetTransform(void) const { return transform; }

void			GameObject::SetActive(bool active)
{
	if (active == _active)
		return ;

	_active = active;

	for (const auto & comp : _components)
		comp->UpdateGameObjectActive();
}

bool			GameObject::IsActive(void) const
{
	if (!_active)
		return false;

	// is active only if all his parents are active
	Transform * t = transform->GetParent();
	while (t != nullptr)
	{
		if (!t->GetGameObject()->IsActive())
			return false;
		t = t->GetParent();
	}

	return true;
}

std::ostream &	operator<<(std::ostream & o, GameObject const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
