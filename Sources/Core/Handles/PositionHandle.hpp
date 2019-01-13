#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Gizmos/Position.hpp"
#include "Core/Handles/BaseHandle.tpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE
#include GLM_INCLUDE_QUATERNION

namespace LWGC::Handles
{
	class		Position : public Gizmo::Position, public BaseHandle< glm::vec3 >
	{
		public:
			Position(const glm::vec3 & position);
			Position(const Position &) = delete;
			virtual ~Position(void);

			bool		HasChanged(void) override;
			glm::vec3	GetDelta(void) override;
			void		Select(void) override;
			void		UnSelect(void) override;

			Position &	operator=(Position const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Position const & r);
}
