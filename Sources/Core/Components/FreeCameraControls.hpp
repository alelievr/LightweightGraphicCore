#pragma once

#include <iostream>
#include <string>

#include "Core/EventSystem.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Object.hpp"

namespace LWGC
{
	class		FreeCameraControls : public Object, public Component
	{
		private:
			float					_forward;
			float					_right;
			float					_up;
			float					_speed;
			float					_mouseSpeed;
			float					_rotationX;
			float					_rotationY;
			KeyDelegateIndex		_keydi;
			MouseMoveDelegateIndex 	_mousemdi;

			virtual void	Update(void) noexcept override;
			void		KeyPressedCallback(KeyCode, ButtonAction);
			void		MouseMovedCallback(glm::vec2 pos, MouseMoveAction action);
			void 		MouseClickCallback(glm::vec2 pos, ButtonAction action);

		public:
			FreeCameraControls(void);
			FreeCameraControls(const FreeCameraControls &) = delete;
			virtual ~FreeCameraControls(void);

			FreeCameraControls &	operator=(FreeCameraControls const & src) = delete;

			virtual void OnEnable() noexcept override;
			virtual void OnDisable() noexcept override;

			virtual void Initialize(void) noexcept override;

			void			SetSpeed(float speed) noexcept;

			virtual uint32_t			GetType(void) const noexcept override;
			static const uint32_t		type = 4;
	};

	std::ostream &	operator<<(std::ostream & o, FreeCameraControls const & r);
}
