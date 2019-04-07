#include "Profiler.hpp"

using namespace LWGC;

ProfilingSamples	Profiler::_samples;
ProfilingEntryData	Profiler::_lastInsertedEntry;

void					Profiler::AddSample(const std::string & name, double durationInMilliseconds, double frameTimeInMilliseconds) noexcept
{
	ProfilingEntryData data = ProfilingEntryData{ durationInMilliseconds, frameTimeInMilliseconds };
	_samples.emplace(name, data);
	_lastInsertedEntry = data;
}

const ProfilingSamples &Profiler::GetSamples(void) noexcept
{
	return _samples;
}

// Useful to retreive the sample that contains all other samples (the frame)
const ProfilingEntryData Profiler::GetLastSample(void) noexcept
{
	return _lastInsertedEntry;
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
