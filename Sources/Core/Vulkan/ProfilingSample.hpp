#pragma once

#include <iostream>
#include <string>
#include <sys/time.h>

#include "IncludeDeps.hpp"
#include "Utils/Color.hpp"
#include "Core/Profiler.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	class		ProfilingSample
	{
		private:
			VkCommandBuffer	_cmd;
			double			_startTime;
			std::string		_sampleName;
			float			_timeFrameRelative;
			bool			_ended;

			static std::string		_hierarchy;

		public:
			ProfilingSample(void) = delete;
			ProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color = Color::Blue);
			ProfilingSample(const std::string & sampleName);
			ProfilingSample(const ProfilingSample &) = delete;
			virtual ~ProfilingSample(void);

			void Insert(const std::string & debugSampleName, const Color & color = Color::Red);
			void End(void);

			ProfilingSample &	operator=(ProfilingSample const & src) = delete;
	};
}
