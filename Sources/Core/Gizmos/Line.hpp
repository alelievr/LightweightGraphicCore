#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Line : public GizmoBase
	{
		private:
			glm::vec3	_p0;
			glm::vec3	_p1;

		public:
			Line(const glm::vec3 p0, const glm::vec3 p1, const Color & c = Color::Red);
			Line(const Line &) = delete;
			virtual ~Line(void);

			void	SetPoint0(const glm::vec3 p);
			void	SetPoint1(const glm::vec3 p);

			Line &	operator=(Line const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Line const & r);
}
