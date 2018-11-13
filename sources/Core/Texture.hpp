#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE

namespace LWGC
{
	class		Texture
	{
		protected:
			int					width;
			int					height;
			int					depth;
			VkFormat			format;
			bool				autoGenerateMips;
			VkImage				image;
			VkDeviceMemory		memory;
			VkImageView			view;

		public:
			Texture(void);
			Texture(const Texture&);
			virtual ~Texture(void);

			virtual Texture &	operator=(Texture const & src);

			int				GetWidth(void) const noexcept;
			int				GetHeight(void) const noexcept;
			int				GetDepth(void) const noexcept;
			VkImageView		GetView(void) const noexcept;
			VkImage			GetImage(void) const noexcept;
			bool			GetAutoGenerateMips(void) const noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Texture const & r);
}
