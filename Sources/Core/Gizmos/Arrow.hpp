#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "Core/Gizmos/Cone.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Arrow : public Cone
	{
		private:
			MeshRenderer *	lineRenderer;
			Material *		lineMaterial;

		public:
			Arrow(float length, int vertices, bool wireframe = true, const Color & c = Color::Red);
			Arrow(const Arrow &) = delete;
			virtual ~Arrow(void);

			void	Initialize(void) noexcept override;

			Arrow &	operator=(Arrow const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Arrow const & r);
}
