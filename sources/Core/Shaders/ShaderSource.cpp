#include "ShaderSource.hpp"

#include <sys/stat.h>
#include <fstream>
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

ShaderSource::ShaderSource(void) : _module(VK_NULL_HANDLE)
{
}

ShaderSource::~ShaderSource(void)
{
	const auto & device = VulkanInstance::Get()->GetDevice();

	if (_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(device, _module, nullptr);
}

std::vector<char> ShaderSource::ReadFile(const std::string & fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	    throw std::runtime_error("failed to open file!");

	size_t fileSize = (size_t) file.tellg();
	std::vector< char > buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

long		ShaderSource::GetFileModificationTime(const std::string & file) const
{
	struct stat st;

	lstat(file.c_str(), &st);

	return st.st_mtimespec.tv_sec;
}

void		ShaderSource::SetSourceFile(const std::string file, const VkShaderStageFlagBits stage)
{
	_sourceFile = ShaderFileInfo{file, GetFileModificationTime(file)};
	SetSource(ReadFile(file), stage);
}

void		ShaderSource::SetSource(const std::vector< char > HLSLSource, const VkShaderStageFlagBits stage)
{
	_stage = stage;

	// TODO: compile HLSL to spirV here

	std::vector<char> spirV;

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = spirV.size();
	createInfo.pCode = reinterpret_cast< const uint32_t* >(spirV.data());

	if (vkCreateShaderModule(VulkanInstance::Get()->GetDevice(), &createInfo, nullptr, &_module) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");
}

bool		ShaderSource::NeedReload(void) const
{
	if (_sourceFile.lastModificationTime != GetFileModificationTime(_sourceFile.path))
		return true;
	else
		return false;
}

VkShaderModule			ShaderSource::GetModule(void) const
{
	return _module;
}

VkShaderStageFlagBits	ShaderSource::GetStage(void) const
{
	return _stage;
}

void		ShaderSource::Reload(void)
{
	std::cout << "TODO" << std::endl;
}

std::ostream &	LWGC::operator<<(std::ostream & o, ShaderSource const & r)
{
	o << "Shader(" << r._sourceFile.path << ")";
	(void)r;
	return (o);
}