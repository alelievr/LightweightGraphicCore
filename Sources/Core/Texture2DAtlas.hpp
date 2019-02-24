#pragma once

# include <iostream>
# include <string>

#include "IncludeDeps.hpp"
#include "Core/Textures/Texture.hpp"

namespace LWGC
{
	class		Texture2DAtlas
	{
		private:
			int		width;
			int		height;
			void*	memory;
			
			void	*findnode(int width, int height);
			// void	*left(mem, w, h);
			// void	*down(mem, w, h);

		public:
			Texture2DAtlas(int width, int height);
			Texture2DAtlas(const Texture2DAtlas&) = delete;

			virtual ~Texture2DAtlas(void);

			// vector of blocks -- texture, width, height
			void	*fit(Texture texture, int width, int height);

			Texture2DAtlas &	operator=(Texture2DAtlas const & src) = delete;
	};
	std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r);
}