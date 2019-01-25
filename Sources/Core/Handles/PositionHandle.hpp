#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Gizmos/Position.hpp"
#include "Core/Handles/BaseHandle.tpp"
#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"
#include "Core/Handles/Slider1DHandle.hpp"

#include GLM_INCLUDE
#include GLM_INCLUDE_QUATERNION

namespace LWGC::Handle
{
	class		Position : public Gizmo::Position, public BaseHandle< glm::vec3 >
	{
		private:
			Slider1D		_upHandle;
			Slider1D		_rightHandle;
			Slider1D		_forwardHandle;

			glm::vec3		_delta;
			bool			_changed;

			void			OnSliderMoved(HandleControl * control, const glm::vec3 delta);
			void			Update(void);

		public:
			Position(const glm::vec3 & position);
			Position(const Position &) = delete;
			virtual ~Position(void);

			bool		HasChanged(void) override;
			glm::vec3	GetDelta(void) override;

			Position &	operator=(Position const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Position const & r);
}
