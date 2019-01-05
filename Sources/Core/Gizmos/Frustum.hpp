#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Frustum : public GizmoBase
	{
		private:
			glm::vec3	_p0;
			glm::vec3	_p1;

		public:
			Frustum(float fovY, float aspect, float nearPlane, float farPlane, const Color & c);
			Frustum(const Frustum &) = delete;
			virtual ~Frustum(void);

			void	SetPoint0(const glm::vec3 p);
			void	SetPoint1(const glm::vec3 p);

			Frustum &	operator=(Frustum const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Frustum const & r);
}
