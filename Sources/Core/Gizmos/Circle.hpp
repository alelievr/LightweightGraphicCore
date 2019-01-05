#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Circle : public GizmoBase
	{
		private:

		public:
			Circle(float radius, int vertices, bool wireframe = true, const Color & c = Color::Red);
			Circle(const Circle &) = delete;
			virtual ~Circle(void);

			// TODO: set radius & vertices

			Circle &	operator=(Circle const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Circle const & r);
}
