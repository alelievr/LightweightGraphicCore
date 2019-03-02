#pragma once

# include <iostream>
# include <string>

#include "IncludeDeps.hpp"
#include "Core/Textures/Texture.hpp"
#include "Core/Vulkan/Vk.hpp"

namespace LWGC
{
	class		Texture2DAtlas : public Texture
	{
		private:
			int			_maxMipLevel;
			void		*memory;
			stbi_uc		*_pixels;

		public:
			Texture2DAtlas(void) = delete;
			Texture2DAtlas(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips);
			Texture2DAtlas(const Texture2DAtlas&) = delete;

			virtual ~Texture2DAtlas(void);

			// vector of blocks -- texture, width, height
			void	*Fit(std::string fileName);

			Texture2DAtlas &	operator=(Texture2DAtlas const & src) = delete;
	};
	std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r);
}