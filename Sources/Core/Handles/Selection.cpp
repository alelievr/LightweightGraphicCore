#include "Selection.hpp"

#include "Core/Application.hpp"

using namespace LWGC;

// Workaround for std::bind because it can't bind members with parameters
#define BIND_CALLBACK_2(x) [&](const auto & _1, const auto & _2){ x(_1, _2); }

Selection::Selection(void) : _selectedGameObject(nullptr), _worldRay(glm::vec3(0, 0, -1))
{
	Application::update.AddListener(std::bind(&Selection::Update, this));
	auto es = Application::Get()->GetEventSystem();

	es->onMouseMove.AddListener(BIND_CALLBACK_2(MouseMoveCallback));
	es->onMouseClick.AddListener(BIND_CALLBACK_2(MouseClickCallback));
}

Selection::~Selection(void)
{
}

void	Selection::Update(void) noexcept
{
	// TODO: get active camera, mouse position and raycast against renderer bounds
	UpdateSelectedObject();

	// TODO: if there is a selected object, display his handles: move, rotate and scale
	UpdateHandles();
}

void	Selection::UpdateSelectedObject(void) noexcept
{

}

void	Selection::UpdateHandles(void) noexcept
{

}

void	Selection::MouseMoveCallback(const glm::vec2 mousePos, const MouseMoveAction action) noexcept
{
	// TODO: update world ray
}

void	Selection::MouseClickCallback(const glm::vec2 mousePos, const ButtonAction action) noexcept
{
	// TODO: update hovered game object
}

GameObject *	Selection::GetSelectedGameObject(void) const noexcept { return _selectedGameObject; }
glm::vec3		Selection::GetWorldRay(void) const noexcept { return _worldRay; }

std::ostream &	operator<<(std::ostream & o, Selection const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
