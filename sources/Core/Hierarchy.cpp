#include "Hierarchy.hpp"

using namespace LWGC;

Hierarchy::Hierarchy(void)
{
}

Hierarchy::~Hierarchy(void)
{
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

const ComponentIndex Hierarchy::RegisterComponentInRenderContext(int componentType, Component * component) noexcept
{
	printf("Register:\n");
	const auto & kp = _renderContext.renderComponents[componentType].insert(component);

	return kp.first;
}

void Hierarchy::UnregisterComponentInRenderContext(int componentType, const ComponentIndex & index) noexcept
{
	_renderContext.renderComponents[componentType].erase(index);
}

const ComponentIndex Hierarchy::RegisterComponentInRenderContext(RenderComponentType componentType, Component * component) noexcept
{
	return RegisterComponentInRenderContext((int)componentType, component);
}

void Hierarchy::UnregisterComponentInRenderContext(RenderComponentType componentType, const ComponentIndex & index) noexcept
{
	UnregisterComponentInRenderContext((int)componentType, index);
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
