#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Gizmos/Position.hpp"
#include "Core/Handles/IHandle.hpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include GLM_INCLUDE
#include GLM_INCLUDE_QUATERNION

namespace LWGC
{
	class		PositionHandle : public Gizmo::Position, public IHandle< glm::vec3 >
	{
		public:
			PositionHandle(const glm::vec3 & position);
			PositionHandle(const PositionHandle &) = delete;
			virtual ~PositionHandle(void);

			bool		HasChanged(void) override;
			glm::vec3	GetDelta(void) override;
			void		Select(void) override;
			void		UnSelect(void) override;

			PositionHandle &	operator=(PositionHandle const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, PositionHandle const & r);
}
