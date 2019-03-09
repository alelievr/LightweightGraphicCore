#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Vulkan/Vk.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Vulkan/CommandBufferPool.hpp"
#include "Core/Object.hpp"
#include VULKAN_INCLUDE

#include STB_INCLUDE_IMAGE

namespace LWGC
{
	class		Texture : public Object
	{
		protected:
			int					width;
			int					height;
			int					depth;
			int					arraySize;
			VkFormat			format;
			bool				autoGenerateMips;
			int					usage;
			bool				allocated;
			int					maxMipLevel;
			VkImage				image;
			VkDeviceMemory		memory;
			VkImageView			view;
			VulkanInstance *	instance;
			VkDevice			device;
			CommandBufferPool *	graphicCommandBufferPool;

			void			AllocateImage(VkImageViewType viewType);
			void			UploadImage(void * pixels, VkDeviceSize imageSize);
			void			UploadImageWithMips(VkImage image, VkFormat format, void * pixels, VkDeviceSize imageSize);
			void			TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
			stbi_uc *		LoadFromFile(const std::string & fileName, int & width, int & height);
			void			GenerateMipMaps(VkImage image, VkFormat format, int32_t width, int32_t height);

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
			void			Destroy(void) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Texture const & r);
}
