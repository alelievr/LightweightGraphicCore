#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Texture.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class		Texture2DArray : public Texture
	{
		private:
			VkBuffer		_stagingBuffer;
			VkDeviceMemory	_stagingBufferMemory;
			void *			_stagingData;
	
		public:
			Texture2DArray(void) = delete;
			Texture2DArray(int width, int height, int arraySize, VkFormat format, int usage);
			Texture2DArray(const Texture2DArray &);
			virtual ~Texture2DArray(void);

			void	SetImage(const std::string & fileName, int targetIndex);
			void	Upload(void);

			int		GetArraySize();
	
			Texture2DArray &	operator=(Texture2DArray const & src);
	};
	
	std::ostream &	operator<<(std::ostream & o, Texture2DArray const & r);
}