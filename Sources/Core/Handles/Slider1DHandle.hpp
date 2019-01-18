#pragma once

#include <iostream>
#include <string>
#include "Core/Handles/HandleControl.hpp"

namespace LWGC::Handle
{
	class		Slider1D : public HandleControl
	{
		private:
			glm::vec3		_worldPoint0;
			glm::vec3		_worldPoint1;

		public:
			Slider1D(void);
			Slider1D(const Slider1D &) = delete;
			virtual ~Slider1D(void);

			void		UpdateWorldPositions(const glm::vec3 & p0, const glm::vec3 & p1);

			void		UpdateSelected(void) override;
			float		UpdateDistance(Camera * cam) override;

			Slider1D &	operator=(Slider1D const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Slider1D const & r);
}
