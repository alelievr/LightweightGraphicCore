#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/CameraType.hpp"
#include "Core/GameObject.hpp"

namespace LWGC
{
	class		Camera : public Object, public Component
	{
		private:
			RenderTarget *	_target;
			glm::vec2		_size;
			CameraType		_cameraType;
			float			_fov;
			float			_nearPlane;
			float			_farPlane;


		public:
			Camera(void);
			Camera(const Camera &) = delete;
			virtual ~Camera(void);

			Camera &	operator=(Camera const & src) = delete;

			virtual void OnAdded(const GameObject &go) noexcept;
			virtual void OnRemoved(const GameObject & go) noexcept;

			glm::vec3	WorldToScreenPoint(glm::vec3 worldPosition);

			glm::vec3	ScreenToWorldPoint(glm::vec3 screenPosition);

			RenderTarget *	GetTarget(void) const;
			void			SetTarget(RenderTarget * tmp);
			
			glm::vec2	GetSize(void) const;
			void		SetSize(glm::vec2 tmp);
			
			CameraType	GetCameraType(void) const;
			void		SetCameraType(CameraType tmp);
			
			float		GetFov(void) const;
			void		SetFov(float tmp);
			
			float		GetNearPlane(void) const;
			void		SetNearPlane(float tmp);
			
			float		GetFarPlane(void) const;
			void		SetFarPlane(float tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Camera const & r);
}
