#include "HandleManager.hpp"

#include "Core/Application.hpp"
#include "Core/Rendering/RenderPipeline.hpp"

#include <cmath>

using namespace LWGC;
using namespace Handle;

#define SELECTION_DISTANCE	0.08f

std::unordered_set< HandleControl * >	HandleManager::_handles;
HandleControl *							HandleManager::_hoveredHandle;
HandleControl *							HandleManager::_selectedHandle;

void		HandleManager::Initialize(void)
{
	auto ev = EventSystem::Get();

	Application::update.AddListener(HandleManager::Update);

	ev->onMouseClick.AddListener(MouseClickCallback);
}

void		HandleManager::Update(void)
{
	UpdateHoveredHandle();

	if (_selectedHandle != nullptr)
		_selectedHandle->UpdateSelected();

	// then if there is a click, call Select on the nearest handle
	// block the distance update until selection wears off
}

void		HandleManager::MouseClickCallback(const glm::vec2 mousePos, int button, ButtonAction action)
{
	(void)mousePos;

	// Handle only use left click currently
	if (button != 0)
		return ;

	if (_hoveredHandle != nullptr && (action == ButtonAction::Press || action == ButtonAction::Repeat))
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
	HandleControl * nearestHandle = nullptr;
	auto cam = RenderPipeline::Get()->GetCurrentCamera();

	// We can't compute distance without a camera
	if (cam == nullptr)
		return ;

	for (auto handleControl : _handles)
	{
		float dist = handleControl->UpdateDistance(cam);
		if (dist < nearest)
			nearestHandle = handleControl;
		nearest = fminf(nearest, dist);
	}

	HandleControl * oldHoveredHandle = _hoveredHandle;
	_hoveredHandle = nullptr;
	if (nearest < SELECTION_DISTANCE)
	{
		_hoveredHandle = nearestHandle;
	}

	if (oldHoveredHandle != _hoveredHandle)
	{
		// Notify the handle that the hover have changed
		if (_hoveredHandle != nullptr)
			_hoveredHandle->onHover.Invoke(_hoveredHandle);
		if (oldHoveredHandle != nullptr)
			oldHoveredHandle->onNormal.Invoke(oldHoveredHandle);
	}
}

void		HandleManager::AddHandleControl(HandleControl * control)
{
	_handles.insert(control);
}

void		HandleManager::RemoveHandleControl(HandleControl * control)
{
	_handles.erase(control);
}