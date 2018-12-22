#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"

namespace LWGC::Gizmo
{
	class		GizmoBase : public GameObject
	{
		private:


		public:
			GizmoBase(void);
			GizmoBase(const GizmoBase &) = delete;
			virtual ~GizmoBase(void);

			GizmoBase &	operator=(GizmoBase const & src) = delete;
	};
}