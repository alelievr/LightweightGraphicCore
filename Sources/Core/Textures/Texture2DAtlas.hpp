#pragma once

# include <iostream>
# include <string>

#include "IncludeDeps.hpp"
#include "Core/NodeTree.hpp"
#include "Core/Textures/Texture.hpp"
#include "Core/Vulkan/Vk.hpp"

#include VULKAN_INCLUDE
#include GLM_INCLUDE

namespace LWGC
{
	class		Texture2DAtlas : public Texture
	{
		private:
			Texture2DAtlas(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips);
			int							_maxMipLevel;
			stbi_uc						*_pixels;
			NodeTree					_nodetree;

			std::vector< glm::vec4 >	_sizeOffsets; // xy: size, zw: offset (UV space)
			glm::vec4					_atlasSize;

			VkBuffer		_sizeOffsetsBuffer;
			VkBuffer		_atlasSizeBuffer;
			VkBufferView	_sizeOffsetBufferView;

			VkDeviceMemory	_sizeOffsetsMemory;
			VkDeviceMemory	_atlasSizeMemory;

		public:
			Texture2DAtlas(void) = delete;
			Texture2DAtlas(const Texture2DAtlas&) = delete;

			virtual ~Texture2DAtlas(void);

			// vector of blocks -- texture, width, height
			static Texture2DAtlas	*Create(uint32_t w, uint32_t h, VkFormat format, int usage, bool allocateMips);
			Rect					Fit(const std::string & fileName);
			void					UploadAtlasDatas(void);

			VkBuffer				GetAtlasSizeBuffer(void);
			VkBuffer				GetSizeOffsetBuffer(void);
			VkBufferView			GetSizeOffsetBufferView(void);
			size_t					GetAtlasSizeBufferSize(void);
			size_t					GetSizeOffsetBufferSize(void);
			void					Clear(void);

			Texture2DAtlas &	operator=(Texture2DAtlas const & src) = delete;
	};
	std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r);
}
