#include "ShaderSource.hpp"

#include <sys/stat.h>
#include <fstream>
#include <unistd.h>
#include "IncludeDeps.hpp"

#include SPIRV_CROSS_INCLUDE

#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

std::vector< std::string > ShaderSource::shaderIncludePaths;

ShaderSource::ShaderSource(void) : _sourceFile({"", 0}), _module(VK_NULL_HANDLE)
{
}

ShaderSource::ShaderSource(const std::string & shaderName, VkShaderStageFlagBits stage)
{
	SetSourceFile(shaderName, stage);
}

ShaderSource::~ShaderSource(void)
{
	const auto & device = VulkanInstance::Get()->GetDevice();

	if (_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device, _module, nullptr);
	}
}

std::vector< char > ShaderSource::ReadFile(const std::string & fileName)
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

void		ShaderSource::SetSourceFile(const std::string & file, const VkShaderStageFlagBits stage)
{
	_stage = stage;
	_sourceFile = ShaderFileInfo{file, GetFileModificationTime(file)};
}

void		ShaderSource::Compile(void)
{
	char path[2048];
	getcwd(path, sizeof(path));
	
	// I gave up using the c++ api of glslang, it's totally unusable
	std::string cmd = "glslangValidator -e main -V -D -S " + StageToText(_stage) + " -I" + path;
	for (const auto & p : shaderIncludePaths)
		cmd += " -I" + p;
	cmd += " " + _sourceFile.path + " -o " + tmpFilePath;
	if (system(cmd.c_str()) != 0)
		throw std::runtime_error("Shader compilation error");

	// Read back spriv from tmp file
	const auto & spirV = ReadFile(tmpFilePath);

	_SpirVCode.resize(spirV.size() / 4);
	std::memcpy(_SpirVCode.data(), spirV.data(), spirV.size());

	// Create Vulkan module
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = spirV.size();
	createInfo.pCode = reinterpret_cast< uint32_t * >(_SpirVCode.data());

	if (vkCreateShaderModule(VulkanInstance::Get()->GetDevice(), &createInfo, nullptr, &_module) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	GenerateShaderBindingTable();
}

void		ShaderSource::GenerateShaderBindingTable(void)
{
	auto glsl = new spirv_cross::CompilerGLSL(_SpirVCode);

	spirv_cross::ShaderResources resources = glsl->get_shader_resources();

	for (auto & resource : resources.uniform_buffers)
	{
		unsigned set = glsl->get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl->get_decoration(resource.id, spv::DecorationBinding);
		// unsigned block = glsl->get_decoration(resource.id, spv::Decoration::);
		printf("Image %s at set = %u, binding = %u, resource name: %s\n", resource.name.c_str(), set, binding, glsl->get_name(resource.id).c_str());
		const spirv_cross::SPIRType & type = glsl->get_type(resource.type_id);
		printf("Is struct: %i\n", (type.basetype == spirv_cross::SPIRType::Struct));
	}

	delete glsl;
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

bool		ShaderSource::HasSource(void) const
{
	return !_sourceFile.path.empty();
}

void		ShaderSource::AddIncludePath(const std::string & path)
{
	shaderIncludePaths.push_back(path);
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