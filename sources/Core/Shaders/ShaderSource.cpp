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

std::vector< uint32_t > ShaderSource::ReadFile(const std::string & fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	    throw std::runtime_error("failed to open file!");

	size_t fileSize = (size_t) file.tellg();
	std::vector< uint32_t > buffer(fileSize + 1); // add one for \0

	file.seekg(0);
	file.read(reinterpret_cast< char * >(buffer.data()), fileSize);

	file.close();

	buffer[fileSize] = 0;

	return buffer;
}

long		ShaderSource::GetFileModificationTime(const std::string & file) const
{
	struct stat st;

	lstat(file.c_str(), &st);

	return st.st_mtimespec.tv_sec;
}

std::string	ShaderSource::StageToText(const VkShaderStageFlagBits stage)
{
	switch (stage)
	{
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return "frag";
		case VK_SHADER_STAGE_VERTEX_BIT:
			return "vert";
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return "comp";
		default:
			throw std::runtime_error("Unhandled stage");
	}
}

void		ShaderSource::SetSourceFile(const std::string file, const VkShaderStageFlagBits stage)
{
	_stage = stage;
	_sourceFile = ShaderFileInfo{file, GetFileModificationTime(file)};
	ReadFile(file);
	
	// I gave up using the c++ api of glslang, it's totally unusable
	std::string cmd = "glslangValidator -e main -V -D -S " + StageToText(stage) + " ";
	cmd += file + " -o " + tmpFilePath;
	printf("cmd: %s\n", cmd.c_str());
	system(cmd.c_str());

	// Read back spriv from tmp file
	_SpirVCode = ReadFile(tmpFilePath);

	printf("spirv length: %i\n", _SpirVCode.size());
}

void		ShaderSource::Compile(void)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = _SpirVCode.size() * 4;
	createInfo.pCode = reinterpret_cast<const uint32_t*>(_SpirVCode.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(VulkanInstance::Get()->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
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