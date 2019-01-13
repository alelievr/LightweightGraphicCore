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
			Slider1D();
			Slider1D(const Slider1D&);
			virtual ~Slider1D(void);

			Slider1D &	operator=(Slider1D const & src);
	};

	std::ostream &	operator<<(std::ostream & o, Slider1D const & r);
}
