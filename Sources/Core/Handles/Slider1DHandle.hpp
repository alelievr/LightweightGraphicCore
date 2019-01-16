#pragma once

#include <iostream>
#include <string>
#include "Core/Handles/IHandleControl.hpp"

namespace LWGC::Handles
{
	class		Slider1D : public IHandleControl
	{
		private:

		public:
			Slider1D(void);
			Slider1D(const Slider1D &) = delete;
			virtual ~Slider1D(void);

			void		UpdateSelected();
			float		UpdateDistance(Camera * cam);

			Slider1D &	operator=(Slider1D const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Slider1D const & r);
}
