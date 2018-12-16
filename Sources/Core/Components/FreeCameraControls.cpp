#include "FreeCameraControls.hpp"

#include "Core/Events/EventSystem.hpp"
#include "Core/Application.hpp"

using namespace LWGC;

FreeCameraControls::FreeCameraControls(void) : _forward(0), _right(0), _up(0), _speed(0.05f), _mouseSpeed(1)
{
}

FreeCameraControls::~FreeCameraControls(void)
{
}

void			FreeCameraControls::OnAdded(const GameObject & go) noexcept
{
	Component::OnAdded(go);
}

void			FreeCameraControls::OnRemoved(const GameObject & go) noexcept
{
	Component::OnRemoved(go);
}

void		FreeCameraControls::Initialize(void) noexcept
{
	Component::Initialize();
}

void		FreeCameraControls::OnEnable(void) noexcept
{
	Component::OnEnable();
}

void		FreeCameraControls::OnDisable(void) noexcept
{
	Component::OnDisable();
}

void		FreeCameraControls::Update(void) noexcept
{
	const auto & event = EventSystem::Get()->GetCurrentEvent();
	const bool keyDown = event.GetType() == EventType::KeyDown;

	switch (event.GetKeyCode())
	{
		case KeyCode::A:
		case KeyCode::LEFT:
			_right = (keyDown) ? -1 : 0;
			break ;
		case KeyCode::D:
		case KeyCode::RIGHT:
			_right = (keyDown) ? 1 : 0;
			break ;
		// Warning: weird thing x and y are swapped but not z, will figure out when i have good debug tools
		case KeyCode::W:
		case KeyCode::UP:
			_forward = (keyDown) ? -1 : 0;
			break ;
		case KeyCode::S:
		case KeyCode::DOWN:
			_forward = (keyDown) ? 1 : 0;
			break ;
		case KeyCode::E:
			_up = (keyDown) ? 1 : 0;
			break ;
		case KeyCode::Q:
			_up = (keyDown) ? -1 : 0;
			break ;
		case KeyCode::KP_ADD:
			_speed *= 1.1f;
			break ;
		case KeyCode::KP_SUBTRACT:
			_speed /= 1.1f;
			break ;
		default:
		break;
	}

	transform->Rotate(glm::vec3(
		event.delta.x,
		event.delta.y,
		0
	) * _mouseSpeed * 0.01f);

	transform->Translate((
		transform->GetRight() * _right
		+ transform->GetUp() * _up
		+ transform->GetForward() * _forward) * _speed);
}

void		FreeCameraControls::SetSpeed(float speed) noexcept { _speed = speed; }

uint32_t	FreeCameraControls::GetType(void) const noexcept
{
	return type;
}

std::ostream &	operator<<(std::ostream & o, FreeCameraControls const & r)
{
	o << "FreeCameraControls" << std::endl;
	(void)r;
	return (o);
}
