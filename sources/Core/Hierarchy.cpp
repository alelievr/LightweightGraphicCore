#include "Hierarchy.hpp"

using namespace LWGC;

Hierarchy::Hierarchy(void)
{
}

Hierarchy::~Hierarchy(void)
{
}

void Hierarchy::AddGameObject(GameObject * gameObject)
{
	_gameObjects.push_back(gameObject);
}

void Hierarchy::RemoveGameObject(GameObject * gameObject)
{
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), gameObject) _gameObjects.end());
}

std::ostream &	operator<<(std::ostream & o, Hierarchy const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
