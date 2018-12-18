#include "Core/Components/Component.hpp"

#include "Core/GameObject.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Transform.hpp"
#include "Core/Application.hpp"

using namespace LWGC;

Component::Component(void) : oldState(false), enabled(false) {}

Component::~Component(void) {}

void    Component::OnAdded(const GameObject & go) noexcept
{
    gameObject = &go;
    hierarchy = gameObject->GetHierarchy();
	transform = gameObject->GetTransform();
}

void    Component::OnRemoved(const GameObject & go) noexcept
{
    gameObject = nullptr;
    hierarchy = nullptr;
	(void)go;
}

void    Component::OnEnable() noexcept
{
	index = hierarchy->RegisterComponent(GetType(), this);
	updateIndex = Application::update.AddListener(std::bind(&Component::Update, this));
}

void    Component::OnDisable() noexcept
{
	hierarchy->UnregisterComponent(index);
	Application::update.RemoveListener(updateIndex);
}

void			Component::Update(void) noexcept {}

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

void			Component::Initialize() noexcept
{
	device = VulkanInstance::Get()->GetDevice();
}

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

Transform *		Component::GetTransform() const noexcept
{
	return transform;
}

uint32_t		Component::GetType() const noexcept
{
	return -1;
}