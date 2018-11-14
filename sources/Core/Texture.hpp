#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Vulkan/Vk.hpp"
#include VULKAN_INCLUDE

#include STB_INCLUDE_IMAGE

namespace LWGC
{
	class		Texture
	{
		protected:
			int					width;
			int					height;
			int					depth;
			int					arraySize;
			VkFormat			format;
			bool				autoGenerateMips;
			VkImage				image;
			VkDeviceMemory		memory;
			VkImageView			view;
			int					usage;
			bool				allocated;
			
			void			AllocateImage();
			void			UploadImage(stbi_uc * pixels, VkDeviceSize imageSize, int targetArrayIndex, int targetMipLevel);
			void			TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

			stbi_uc *		LoadFromFile(const std::string & fileName, int & width, int & height);

		public:
			Texture(void);
			Texture(const Texture&);
			virtual ~Texture(void);

			virtual Texture &	operator=(Texture const & src);

			int				GetWidth(void) const noexcept;
			int				GetHeight(void) const noexcept;
			int				GetDepth(void) const noexcept;
			int				GetArraySize(void) const noexcept;
			VkImageView		GetView(void) const noexcept;
			VkImage			GetImage(void) const noexcept;
			bool			GetAutoGenerateMips(void) const noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Texture const & r);
}
