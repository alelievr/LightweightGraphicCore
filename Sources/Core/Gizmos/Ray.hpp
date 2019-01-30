#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"
#include "Core/Gizmos/Line.hpp"

#include GLM_INCLUDE

namespace LWGC::Gizmo
{
	class		Ray : public Line
	{
		private:
			glm::vec3	_origin;

		public:
			Ray(const glm::vec3 & origin, const glm::vec3 & direction, const Color & c = Color::Red);
			Ray(const Ray &) = delete;
			virtual ~Ray(void);

			void	SetOrigin(const glm::vec3 & origin);
			void	SetDirection(const glm::vec3 & direction);

			Ray &	operator=(Ray const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Ray const & r);
}
