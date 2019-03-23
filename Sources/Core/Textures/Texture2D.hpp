#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Textures/Texture.hpp"
#include "Core/Vulkan/Vk.hpp"

#include VULKAN_INCLUDE
#include GLM_INCLUDE

namespace LWGC
{
	class		Texture2D : public Texture
	{
		private:
			Texture2D(void) = delete;
			Texture2D(const std::string fileName, VkFormat format, int usage, bool generateMips = false);
			Texture2D(unsigned width, unsigned height, VkFormat format, int usage, void *data, unsigned size, bool generateMips = false);
			Texture2D(std::size_t width, std::size_t height, VkFormat format, int usage, bool allocateMips = false);
			Texture2D(const Texture2D &);

			std::string		_name;
			stbi_uc *		_pixels;

		public:
			static Texture2D *Create(const std::string fileName, VkFormat format, int usage, bool generateMips = false);
			static Texture2D *Create(std::size_t width, std::size_t height, VkFormat format, int usage, bool allocateMips = false);
			static Texture2D *Create(const Texture2D &);

			virtual ~Texture2D(void);

			virtual Texture2D &	operator=(Texture2D const & src);

			void	Apply(void);
			void	SetPixel(int x, int y);
			void	GetPixel(int x, int y);

			void	SetName(const std::string & name) override;
	};

	std::ostream &	operator<<(std::ostream & o, Texture2D const & r);
}
