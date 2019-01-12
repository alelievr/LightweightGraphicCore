#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Gizmos/Arrow.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE
#include GLM_INCLUDE_QUATERNION

namespace LWGC::Gizmo
{
	class		Position : public GameObject
	{
		private:
			Arrow *		upArrow;
			Arrow *		rightArrow;
			Arrow *		forwardArrow;

		public:
			Position(const glm::vec3 & position);
			Position(const Position &) = delete;
			virtual ~Position(void);

			void	Initialize(void) noexcept override;

			Position &	operator=(Position const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Position const & r);
}
