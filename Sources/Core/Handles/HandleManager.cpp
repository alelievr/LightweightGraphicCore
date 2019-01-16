#include "HandleManager.hpp"

#include "Core/Application.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"

#include <cmath>

using namespace LWGC;
using namespace Handles;

#define SELECTION_DISTANCE	0.2f

std::unordered_set< IHandleControl * >		HandleManager::_handles;
IHandleControl *							HandleManager::_hoveredHandle;
IHandleControl *							HandleManager::_selectedHandle;

void		HandleManager::Initialize(void)
{
	auto ev = EventSystem::Get();

	Application::update.AddListener(HandleManager::Update);

	ev->onMouseClick.AddListener(MouseClickCallback);
}

void		HandleManager::Update(void)
{
	if (_selectedHandle == nullptr)
		UpdateHoveredHandle();
	else
		_selectedHandle->UpdateSelected(); // TODO

	// then if there is a click, call Select on the nearest handle
	// block the distance update until selection wears off
}

void		HandleManager::MouseClickCallback(const glm::vec2 mousePos, int button, ButtonAction action)
{
	if (_hoveredHandle != nullptr)
	{
		_selectedHandle = _hoveredHandle;

		_selectedHandle->onSelected.Invoke(_selectedHandle);
	}
	else
		_selectedHandle = nullptr;
}

void		HandleManager::UpdateHoveredHandle(void)
{
	float nearest = 1e20;
	IHandleControl * nearestHandle = nullptr;
	auto cam = VulkanRenderPipeline::Get()->GetCurrentCamera();

	for (auto handleControl : _handles)
	{
		float dist = handleControl->UpdateDistance(cam);
		if (dist < nearest)
			nearestHandle = handleControl;
		nearest = fminf(nearest, dist);
	}

	_hoveredHandle = nullptr;
	if (nearest < SELECTION_DISTANCE)
	{
		_hoveredHandle = nearestHandle;
	}
}