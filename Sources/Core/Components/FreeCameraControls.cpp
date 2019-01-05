#include "FreeCameraControls.hpp"


#include "Core/Application.hpp"
#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

FreeCameraControls::FreeCameraControls(void) : _forward(0), _right(0), _up(0), _speed(0.05f), _mouseSpeed(0.2f), _rotationX(0), _rotationY(0)
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

	EventSystem::Get()->LockCursor();
}

using keyDelegateIndex = DelegateIndex<void (LWGC::KeyCode, LWGC::ButtonAction)>;

void		FreeCameraControls::OnEnable(void) noexcept
{
	Component::OnEnable();
	// save index
	keyDelegateIndex index = EventSystem::Get()->onKey.AddListener([&](auto k, auto a){KeyPressedCallback(k, a);});
	EventSystem::Get()->onMouseMove.AddListener([&](auto k, auto a){MouseMoveedCallback(k, a);});
	// EventSystem::Get()->onMouseClick.AddListener();
}

void		FreeCameraControls::OnDisable(void) noexcept
{
	Component::OnDisable();
	// index
	// EventSystem::Get()->onKey.RemoveListener(index);
	// EventSystem::Get()->onMouseClick.RemoveListener();
	// EventSystem::Get()->onMouseMove.RemoveListener();
}

void		FreeCameraControls::MouseMoveedCallback(glm::vec2 pos, MouseMoveAction action)
{
	const auto & event = EventSystem::Get();
	
	_rotationX += event->delta.x * _mouseSpeed;
	_rotationY += event->delta.y * _mouseSpeed;

	_rotationY = std::clamp(_rotationY, -90.0f, 90.0f);
	transform->SetRotation(glm::angleAxis(_rotationX * Math::DegToRad, glm::vec3(0, 1, 0)));
	transform->RotateAxis(_rotationY * Math::DegToRad, glm::vec3(1, 0, 0));

	// switch (key)
	// {
	// 	case KeyCode::A:
	// 	default:
	// 	break;
	// }
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
			EventSystem::Get()->ToggleLockCursor();
		default:
		break;
	}
}

void		FreeCameraControls::Update(void) noexcept
{
	const auto & event = EventSystem::Get();

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
