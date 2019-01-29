#pragma once

#include <iostream>
#include <string>

#include "Core/Textures/Texture.hpp"

namespace LWGC
{
	class		TextureCube : public Texture
	{
		private:
			TextureCube(void) = default;
			TextureCube(const TextureCube&);
	
		public:
			TextureCube*	Create(void);
			TextureCube*	Create(const TextureCube&);
			virtual ~TextureCube(void) = default;
	
			TextureCube &	operator=(TextureCube const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, TextureCube const & r);
}
