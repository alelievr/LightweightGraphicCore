#include "Profiler.hpp"

using namespace LWGC;

ProfilingSamples	Profiler::_samples;

void					Profiler::AddSample(const std::string & name, double durationInMilliseconds, double frameTimeInMilliseconds) noexcept
{
	_samples.emplace(name, ProfilingEntryData{ durationInMilliseconds, frameTimeInMilliseconds });
}

const ProfilingSamples &Profiler::GetSamples(void) noexcept
{
	return _samples;
}

void					Profiler::Reset(void) noexcept
{
	_samples.clear();
}

std::ostream &	operator<<(std::ostream & o, Profiler const & r)
{
	o << "Profiler" << std::endl;
	(void)r;
	return (o);
}
