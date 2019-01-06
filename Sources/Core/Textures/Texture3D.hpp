#pragma once

#include <iostream>
#include <string>

#include "Core/Textures/Texture.hpp"

namespace LWGC
{
	class		Texture3D : public Texture
	{
		private:
			Texture3D(void) = delete;
			Texture3D(std::size_t width, std::size_t height, std::size_t depth, VkFormat format, int usage);
			Texture3D(const Texture3D &);

		public:
			static Texture3D *Create(std::size_t width, std::size_t height, std::size_t depth, VkFormat format, int usage);
			static Texture3D *Create(const Texture3D &);
			virtual ~Texture3D(void) = default;
	
			Texture3D &	operator=(Texture3D const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, Texture3D const & r);
}
