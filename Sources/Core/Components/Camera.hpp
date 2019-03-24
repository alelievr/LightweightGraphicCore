#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Core/CameraType.hpp"
#include "Core/GameObject.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Vulkan/DescriptorSet.hpp"

namespace LWGC
{
	class		Camera : public Object, public Component
	{
		private:
			struct LWGC_PerCamera
			{
				glm::mat4	projection;
				glm::mat4	view;
				glm::vec4	positionWS;
				glm::vec4	screenSize;
			};

			RenderTarget *			_target;
			glm::vec2				_viewportSize;
			CameraType				_cameraType;
			float					_fov;
			float					_nearPlane;
			float					_farPlane;
			UniformBuffer			_uniformCameraBuffer;
			LWGC_PerCamera			_perCamera;
			DescriptorSet			_perCameraSet;

			void							UpdateUniformData(void) noexcept;
			virtual void					Update(void) noexcept override;

		public:
			Camera(void);
			Camera(const Camera &) = delete;
			virtual ~Camera(void);

			Camera &	operator=(Camera const & src) = delete;

			virtual void OnEnable(void) noexcept override;
			virtual void OnDisable(void) noexcept override;

			virtual void Initialize(void) noexcept override;

			glm::vec3	WorldToScreenPoint(glm::vec3 worldPosition);

			glm::vec3	ScreenToWorldPoint(glm::vec3 screenPosition);

			RenderTarget *	GetTarget(void) const;
			void			SetTarget(RenderTarget * tmp);

			glm::vec2	GetViewportSize(void) const;

			CameraType	GetCameraType(void) const;
			void		SetCameraType(CameraType tmp);

			float		GetFov(void) const;
			void		SetFov(float tmp);

			float		GetNearPlane(void) const;
			void		SetNearPlane(float tmp);

			float		GetFarPlane(void) const;
			void		SetFarPlane(float tmp);

			glm::mat4	GetViewMatrix(void) const;
			glm::mat4	GetProjectionMatrix(void) const;

			VkDescriptorSet		GetDescriptorSet(void);

			virtual uint32_t	GetType(void) const noexcept override;

			static const uint32_t		type = 2;
	};

	std::ostream &	operator<<(std::ostream & o, Camera const & r);
}
