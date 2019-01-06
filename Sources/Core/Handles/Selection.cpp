#include "Selection.hpp"

#include "Core/Application.hpp"
#include "Core/Components/Camera.hpp"
#include "Core/Components/Renderer.hpp"
#include "Utils/Vector.hpp"
#include "Core/Gizmos/Cone.hpp"
#include "Core/Gizmos/GizmoBase.hpp"
#include "Utils/Color.hpp"

using namespace LWGC;

// Workaround for std::bind because it can't bind members with parameters
#define BIND_CALLBACK_2(x) [&](const auto & _1, const auto & _2){ x(_1, _2); }
#define BIND_CALLBACK_3(x) [&](const auto & _1, const auto & _2, const auto & _3){ x(_1, _2, _3); }

Selection::Selection(void) : _selectedGameObject(nullptr), _worldRay(glm::vec3(0, 0, -1))
{
	Application::update.AddListener(std::bind(&Selection::Update, this));
	_eventSystem = Application::Get()->GetEventSystem();
	_renderContext = Application::Get()->GetHierarchy()->GetRenderContext();

	_eventSystem->onMouseClick.AddListener(BIND_CALLBACK_3(MouseClickCallback));
}

Selection::~Selection(void)
{
}

void	Selection::Update(void) noexcept
{
	Camera *		cam = VulkanRenderPipeline::Get()->GetCurrentCamera();

	// If there is no camera, we don't have to raycast
	if (cam == nullptr)
		return ;

	UpdateWorldRay(cam);
	UpdateSelectedObject(cam);
	UpdateHandles();
}

void	Selection::UpdateSelectedObject(Camera * cam) noexcept
{
	std::unordered_set< Renderer * >	renderers;
	glm::vec3							origin = cam->GetTransform()->GetPosition();

	_renderContext->GetRenderers(renderers);
	_hoveredGameObject = nullptr;

	// TODO: multi-object ?
	for (const auto & renderer : renderers)
	{
		// Ignore gizmos in raycast, TODO: gizmos must be in a separate list !
		if (dynamic_cast< Gizmo::GizmoBase * >(renderer->GetGameObject()))
			continue ;

		if (renderer->GetBounds().Intersects(origin, _worldRay))
		{
			_hoveredGameObject = renderer->GetGameObject();
			break ;
		}
	}
}

void	Selection::UpdateHandles(void) noexcept
{

}

void	Selection::UpdateWorldRay(Camera * cam) noexcept
{
	const glm::vec2	viewportPosition = _eventSystem->GetCursorPosition();

	// If there is no camera, we don't have to raycast
	if (cam == nullptr)
		return ;

	glm::vec2 ndcPosition = viewportPosition / cam->GetViewportSize() * 2.0f - 1.0f;
	glm::vec4 clipDirection = glm::vec4(ndcPosition, 1, 0); // z = -1 because we cast a ray forward to the camera
	glm::vec4 eyeDirection = cam->GetProjectionMatrix() * clipDirection;
	// Again, forward and 0 for homogenous direction
	eyeDirection.z = 1;
	eyeDirection.w = 0;

	_worldRay = glm::vec3((glm::inverse(cam->GetViewMatrix()) * eyeDirection));
	_worldRay = glm::normalize(_worldRay);
}

void	Selection::MouseClickCallback(const glm::vec2 mousePos, int button, const ButtonAction action) noexcept
{
	// left click select
	if (button == 0)
	{
		_selectedGameObject = _hoveredGameObject;

		if (_selectedGameObject != nullptr)
			std::cout << "Selecting object at position: " << _selectedGameObject->GetTransform()->GetPosition() << std::endl;
	}
}

GameObject *	Selection::GetSelectedGameObject(void) const noexcept { return _selectedGameObject; }
GameObject *	Selection::GetHoveredGameObject(void) const noexcept { return _hoveredGameObject; }
glm::vec3		Selection::GetWorldRay(void) const noexcept { return _worldRay; }

std::ostream &	operator<<(std::ostream & o, Selection const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
