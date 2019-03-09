#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class		ProfilingSample
	{
		private:
			VkCommandBuffer	_cmd;

		public:
			ProfilingSample(void) = delete;
			ProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color = Color::Blue);
			ProfilingSample(const ProfilingSample &) = delete;
			virtual ~ProfilingSample(void);

			void Insert(const std::string & debugSampleName, const Color & color = Color::Red);

			ProfilingSample &	operator=(ProfilingSample const & src) = delete;
	};
}
