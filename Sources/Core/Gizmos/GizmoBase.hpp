#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Utils/Color.hpp"

namespace LWGC::Gizmo
{
	class		GizmoBase : public GameObject
	{
		protected:
			struct LWGC_GizmoData
			{
				glm::vec4	color;
				int		colorMode;
			};

			Material *		material;
			MeshRenderer *	renderer;
			Color			color;
			VkBuffer		gizmoDataBuffer;
			VkDeviceMemory	gizmoDataMemory;
			LWGC_GizmoData	gizmoData;

			virtual void Initialize(void) noexcept override;

		public:
			GizmoBase(void);
			GizmoBase(const Color & c);
			GizmoBase(const GizmoBase &) = delete;
			virtual ~GizmoBase(void);

			void	SetColor(const Color & color);

			GizmoBase &	operator=(GizmoBase const & src) = delete;
	};
}