#include "Hierarchy.hpp"

#include "Core/GameObject.hpp"

using namespace LWGC;

Hierarchy::Hierarchy(void)
{
}

Hierarchy::~Hierarchy(void)
{
	std::cout << "Hierarchy destructed" << std::endl;

	for (auto & comps : _components)
		comps->~Component();
}

void Hierarchy::Initialize(void)
{
	for (const auto & go : _gameObjects)
	{
		go->Initialize();
	}
}

void Hierarchy::AddGameObject(GameObject * gameObject)
{
	gameObject->SetHierarchy(this);
	_gameObjects.push_back(gameObject);
}

void Hierarchy::RemoveGameObject(GameObject * gameObject)
{
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject), _gameObjects.end());
}

GameObject * Hierarchy::GetGameObject(int index)
{
	return _gameObjects[index];
}

const ComponentIndex	Hierarchy::RegisterComponent(uint32_t componentType, Component * component) noexcept
{
	if (componentType == Camera::type)
	{
		_cameras.push_back(dynamic_cast< Camera * >(component));
	}

	return _components.insert(component).first;
}

void					Hierarchy::UnregisterComponent(const ComponentIndex & index) noexcept
{
	_components.erase(index);
}

std::vector< Camera * >	Hierarchy::GetCameras(void) noexcept
{
	return _cameras;
}

const ComponentIndex Hierarchy::RegisterComponentInRenderContext(uint32_t componentType, Component * component) noexcept
{
	const auto & kp = _renderContext.renderComponents[componentType].insert(component);

	return kp.first;
}

void Hierarchy::UnregisterComponentInRenderContext(uint32_t componentType, const ComponentIndex & index) noexcept
{
	_renderContext.renderComponents[componentType].erase(index);
}

RenderContext &	Hierarchy::GetRenderContext(void)
{
	return _renderContext;
}

std::ostream &	operator<<(std::ostream & o, Hierarchy const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
