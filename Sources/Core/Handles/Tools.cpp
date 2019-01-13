#include "Tools.hpp"

#include "Core/Application.hpp"
#include "Core/Handles/Selection.hpp"

using namespace LWGC;

int					Tools::_toolIndex = static_cast< int >(BuiltinTools::Position);
PositionHandle *	Tools::_positionHandle = nullptr;
Hierarchy *			Tools::_hierarchy = nullptr;

int			Tools::GetCurrentToolIndex(void) { return _toolIndex; }

void		Tools::Initialize(void) noexcept
{
	_hierarchy = Application::Get()->GetHierarchy();
	_positionHandle = new PositionHandle(glm::vec3(0, 0, 0));
	_positionHandle->SetActive(false);
	_hierarchy->AddGameObject(_positionHandle);

	Application::update.AddListener(Tools::Update);
}

void		Tools::Update(void) noexcept
{
	auto selectedGameObject = Selection::GetSelectedGameObject();

	if (selectedGameObject == nullptr)
	{
		if (_positionHandle->IsActive())
			_positionHandle->SetActive(false);
		return ;
	}

	if (!_positionHandle->IsActive())
		_positionHandle->SetActive(true);

	// TODO: table of IHandle
	if (_positionHandle->HasChanged())
		selectedGameObject->GetTransform()->Translate(_positionHandle->GetDelta());
}