#include "Core/Components/Component.hpp"

#include "Core/GameObject.hpp"
#include "Core/Hierarchy.hpp"

using namespace LWGC;

Component::Component(void) : oldState(false), enabled(false) {}

Component::~Component(void) {}

void    Component::OnAdded(const GameObject & go) noexcept
{
    gameObject = &go;
    hierarchy = gameObject->GetHierarchy();
}

void    Component::OnRemoved(const GameObject & go) noexcept
{
    gameObject = nullptr;
    hierarchy = nullptr;
	(void)go;
}

void    Component::OnEnable() noexcept {}

void    Component::OnDisable() noexcept {}

void			Component::UpdateGameObjectActive(void) noexcept
{
	if (gameObject->IsActive())
	{
		oldState = enabled;
		if (!enabled)
			OnEnable();
	}
	else if (oldState)
	{
		if (enabled)
			OnDisable();
		enabled = oldState;
	}
}

void			Component::Initialize() noexcept {}

void			Component::Enable() noexcept
{
	if (!enabled && gameObject->IsActive())
		OnEnable();
	oldState = enabled = true;
}

void			Component::Disable() noexcept
{
	if (enabled && gameObject->IsActive())
		OnDisable();
	oldState = enabled = false;
}

bool            Component::IsEnabled() noexcept
{
    return enabled && gameObject->IsActive();
}
