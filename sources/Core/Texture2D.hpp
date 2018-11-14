#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Texture.hpp"
#include "Core/Vulkan/Vk.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class		Texture2D : Texture
	{
		private:
			std::string		_name;

		public:
			Texture2D(void) = delete;
			Texture2D(const std::string fileName, VkFormat format, int usage);
			Texture2D(std::size_t width, std::size_t height, VkFormat format, int usage);
			Texture2D(const Texture2D &);
			virtual ~Texture2D(void);

			virtual Texture2D &	operator=(Texture2D const & src);

			void	Apply(void);
			void	SetPixel(int x, int y);
			void	GetPixel(int x, int y);
	};

	std::ostream &	operator<<(std::ostream & o, Texture2D const & r);
}
