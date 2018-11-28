#include "ShaderSource.hpp"

#include <sys/stat.h>
#include <fstream>
#include "Core/Vulkan/VulkanInstance.hpp"

#include GLSLANG_RESOURCES_INCLUDE

using namespace LWGC;

ShaderSource::ShaderSource(void) : _module(VK_NULL_HANDLE), _shader(nullptr)
{
}

ShaderSource::~ShaderSource(void)
{
	const auto & device = VulkanInstance::Get()->GetDevice();

	if (_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(device, _module, nullptr);
	
	if (_shader != nullptr)
		delete _shader;
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

EShLanguage	ShaderSource::ShaderStageToLang(const VkShaderStageFlagBits stage)
{
	switch (stage)
	{
		case VK_SHADER_STAGE_VERTEX_BIT:
			return EShLangVertex;
			break ;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return EShLangFragment;
			break ;
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return EShLangCompute;
			break ;
		default:
			throw std::runtime_error("Can't compile the shader, unknown stage");
	}
}

void		ShaderSource::SetSource(const std::vector< char > HLSLSource, const VkShaderStageFlagBits stage)
{
	_stage = stage;

	const char *						data = HLSLSource.data();
	EShLanguage							language = ShaderStageToLang(stage);
	glslang::EShTargetClientVersion		targetVersion = glslang::EShTargetVulkan_1_1;
	glslang::EShTargetLanguageVersion	targetLanguageVersion = glslang::EShTargetSpv_1_3;

	_shader = new glslang::TShader(language);
	_shader->setStrings(&data , 1);
	_shader->setAutoMapLocations(true);
	_shader->setAutoMapBindings(true);
	_shader->setEnvInput(glslang::EShSourceHlsl, language, glslang::EShClientVulkan, 650);
	_shader->setEnvClient(glslang::EShClientVulkan, targetVersion);
	_shader->setEnvTarget(glslang::EShTargetSpv, targetLanguageVersion);
	
	_shader->parse(&glslang::DefaultTBuiltInResource, 650, false, EShMsgDefault);

	// TODO: do this in another function
	_program.addShader(_sahder);

	success &= program.link(controls);
	success &= program.mapIO();

	std::vector<uint32_t>	spirv_binary;
	glslang::SpvOptions		options;
	options.validate = true;
	spv::SpvBuildLogger		logger;
	glslang::GlslangToSpv(*_program.getIntermediate(language),
						  spirv_binary, &logger, &options);

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

glslang::TShader *		ShaderSource::GetShader(void) const
{
	return _shader;
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