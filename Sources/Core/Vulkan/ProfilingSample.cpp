#include "ProfilingSample.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

ProfilingSample::ProfilingSample(VkCommandBuffer cmd, const std::string & debugSampleName, const Color & color) : _cmd(cmd)
{
	Vk::BeginProfilingSample(cmd, debugSampleName, color);
}

void	ProfilingSample::Insert(const std::string & debugSampleName, const Color & color)
{
	Vk::InsertProfilingSample(_cmd, debugSampleName, color);
}

ProfilingSample::~ProfilingSample(void)
{
	Vk::EndProfilingSample(_cmd);
}
