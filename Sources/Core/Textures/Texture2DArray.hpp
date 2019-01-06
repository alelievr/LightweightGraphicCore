#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IncludeDeps.hpp"
#include "Core/Textures/Texture.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class		Texture2DArray : public Texture
	{
		private:
			Texture2DArray(void) = delete;
			Texture2DArray(int width, int height, int arraySize, VkFormat format, int usage);
			Texture2DArray(const Texture2DArray &);

			VkBuffer							_stagingBuffer;
			VkDeviceMemory						_stagingBufferMemory;
			void *								_stagingData;
			std::vector< VkBufferImageCopy >	_bufferCopyRegions;
	
		public:
			static Texture2DArray *Create(int width, int height, int arraySize, VkFormat format, int usage);
			static Texture2DArray *Create(const Texture2DArray &);
			virtual ~Texture2DArray(void);

			void	SetImage(const std::string & fileName, int targetIndex);
			void	Upload(void);

			int		GetArraySize();
	
			Texture2DArray &	operator=(Texture2DArray const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, Texture2DArray const & r);
}