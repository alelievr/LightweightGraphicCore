#pragma once

#include <iostream>
#include <string>

#include "Core/Textures/Texture.hpp"

namespace LWGC
{
	class		TextureCube : public Texture
	{
		private:
	
	
		public:
			TextureCube();
			TextureCube(const TextureCube&);
			virtual ~TextureCube(void);
	
			TextureCube &	operator=(TextureCube const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, TextureCube const & r);
}
