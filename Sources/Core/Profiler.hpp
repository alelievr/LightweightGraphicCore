#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Core/Vulkan/ProfilingSample.hpp"

namespace LWGC
{
	struct		ProfilingEntryData
	{
		double	duration;	// in milliseconds
		double	time;		// in milliseconds
	};

	using ProfilingSamples = std::unordered_multimap< std::string, ProfilingEntryData >;

	class		Profiler
	{
		friend class ProfilingSample;
		private:
			static ProfilingSamples	_samples;

			static void	AddSample(const std::string & name, double durationInMilliseconds, double frameTimeInMilliseconds) noexcept;

		public:
			Profiler(void) = delete;
			Profiler(const Profiler &) = delete;
			virtual ~Profiler(void) = delete;

			Profiler &	operator=(Profiler const & src) = delete;

			static const ProfilingSamples &	GetSamples(void) noexcept;
			static void		Reset(void) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Profiler const & r);
}