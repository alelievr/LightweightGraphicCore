#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Components/MeshRenderer.hpp"

namespace LWGC::Gizmo
{
	class		GizmoBase : public GameObject
	{
		protected:
			Material *		material;
			MeshRenderer *	renderer;

		public:
			GizmoBase(void);
			GizmoBase(const GizmoBase &) = delete;
			virtual ~GizmoBase(void);

			GizmoBase &	operator=(GizmoBase const & src) = delete;
	};
}