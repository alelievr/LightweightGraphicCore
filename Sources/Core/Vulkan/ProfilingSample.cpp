#include "ProfilingSample.hpp"

#include "Core/Vulkan/Vk.hpp"
#include "Core/Time.hpp"

using namespace LWGC;

std::string ProfilingSample::_hierarchy;

ProfilingSample::ProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color) : ProfilingSample(debugSampleName)
{
	Vk::BeginProfilingSample(cmd, debugSampleName, color);
}

ProfilingSample::ProfilingSample(const std::string & sampleName) : _cmd(VK_NULL_HANDLE), _sampleName(sampleName), _ended(false)
{
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	_startTime = (double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0;

	_timeFrameRelative = Time::GetFrameRelativeTime();

	_hierarchy += "/" + sampleName;
}

void	ProfilingSample::Insert(const std::string & debugSampleName, const Color & color)
{
	Vk::InsertProfilingSample(_cmd, debugSampleName, color);
}

void	ProfilingSample::End(void)
{
	if (_ended)
		return;

	struct timeval  tv;

	Vk::EndProfilingSample(_cmd);

	gettimeofday(&tv, NULL);
	double endTime = (double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0;

	Profiler::AddSample(_hierarchy, endTime - _startTime, _timeFrameRelative);

	_hierarchy = _hierarchy.substr(0, _hierarchy.size() - _sampleName.size() - 1);

	_ended = true;
}

ProfilingSample::~ProfilingSample(void)
{
	End();
}
