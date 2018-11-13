#pragma once

#include <iostream>
#include <string>

#include "Core/Texture.hpp"

namespace LWGC
{
	class		Texture3D : public Texture
	{
		private:
	
	
		public:
			Texture3D();
			Texture3D(const Texture3D &);
			virtual ~Texture3D(void);
	
			Texture3D &	operator=(Texture3D const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, Texture3D const & r);
}
