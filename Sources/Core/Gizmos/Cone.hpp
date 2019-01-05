#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Cone : public GizmoBase
	{
		private:

		public:
			Cone(float radius, float height, int vertices, bool wireframe = true, const Color & c = Color::Red);
			Cone(const Cone &) = delete;
			virtual ~Cone(void);

			// TODO: set radius & vertices

			Cone &	operator=(Cone const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Cone const & r);
}
