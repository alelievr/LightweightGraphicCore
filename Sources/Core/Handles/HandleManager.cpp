#include "HandleManager.hpp"

#include "Core/Application.hpp"

using namespace LWGC;

void		HandleManager::Initialize(void)
{
	Application::update.AddListener(HandleManager::Update);
}

void		HandleManager::Update(void)
{
	// TODO: update every handle distance from the mouse
	// then if there is a click, call Select on the nearest handle
	// block the distance update until selection wears off
}
