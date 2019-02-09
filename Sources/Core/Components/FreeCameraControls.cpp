#include "Core/Components/FreeCameraControls.hpp"

#include <cmath>

#include "Core/Application.hpp"
#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"
#include "Core/Time.hpp"

using namespace LWGC;

FreeCameraControls::FreeCameraControls(void) : _forward(0), _right(0), _up(0), _speed(5.0f), _mouseSpeed(0.2f), _rotationX(0), _rotationY(0)
{
}

FreeCameraControls::~FreeCameraControls(void)
{
}

void		FreeCameraControls::Initialize(void) noexcept
{
	Component::Initialize();

	EventSystem::Get()->LockCursor();
}

void		FreeCameraControls::OnEnable(void) noexcept
{
	Component::OnEnable();
	_keydi = EventSystem::Get()->onKey.AddListener([&](auto k, auto a){KeyPressedCallback(k, a);});
	_mousemdi = EventSystem::Get()->onMouseMove.AddListener([&](auto k, auto a){MouseMovedCallback(k, a);});
}

void		FreeCameraControls::OnDisable(void) noexcept
{
	Component::OnDisable();
	EventSystem::Get()->onKey.RemoveListener(_keydi);
	EventSystem::Get()->onMouseMove.RemoveListener(_mousemdi);
}

void		FreeCameraControls::MouseClickCallback(glm::vec2 pos, ButtonAction action)
{
	(void)pos;
	(void)action;
}

void		FreeCameraControls::MouseMovedCallback(glm::vec2 pos, MouseMoveAction action)
{
	(void)pos;

	// Don't move camera if the cursor is locked
	if (!EventSystem::Get()->IsCursorLocked())
		return ;

	if (action == MouseMoveAction::Move)
	{
		const auto & event = EventSystem::Get();

		_rotationX += event->delta.x * _mouseSpeed;
		_rotationY += event->delta.y * _mouseSpeed;
		_rotationY = glm::clamp(_rotationY, -90.0f, 90.0f);

		transform->SetRotation(glm::angleAxis(_rotationX * Math::DegToRad, glm::vec3(0, 1, 0)));
		transform->RotateAxis(_rotationY * Math::DegToRad, glm::vec3(1, 0, 0));
	}
}

void		FreeCameraControls::KeyPressedCallback(KeyCode key, ButtonAction action)
{
	bool buttonPress = action == ButtonAction::Press || action == ButtonAction::Repeat;

	switch (key)
	{
		case KeyCode::A:
		case KeyCode::LEFT:
			_right = buttonPress ? -1 : 0;
			break ;
		case KeyCode::D:
		case KeyCode::RIGHT:
			_right = buttonPress ? 1 : 0;
			break ;
		case KeyCode::W:
		case KeyCode::UP:
			_forward = buttonPress ? 1 : 0;
			break ;
		case KeyCode::S:
		case KeyCode::DOWN:
			_forward = buttonPress ? -1 : 0;
			break ;
		case KeyCode::E:
			_up = buttonPress ? 1 : 0;
			break ;
		case KeyCode::Q:
			_up = buttonPress ? -1 : 0;
			break ;
		case KeyCode::KP_ADD:
			_speed *= 1.1f;
			break ;
		case KeyCode::KP_SUBTRACT:
			_speed /= 1.1f;
			break ;
		case KeyCode::SPACE:
			if (action == ButtonAction::Press)
				EventSystem::Get()->ToggleLockCursor();
			break ;
		default:
		break;
	}
}

void		FreeCameraControls::Update(void) noexcept
{
	transform->Translate((
		transform->GetRight() * _right
		+ transform->GetUp() * _up
		+ transform->GetForward() * _forward) * _speed * Time::GetDeltaTime());
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
