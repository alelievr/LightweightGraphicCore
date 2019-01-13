#include "ShaderSource.hpp"

#include <sys/stat.h>
#include <fstream>
#include <unistd.h>
#include "IncludeDeps.hpp"

#include SPIRV_CROSS_INCLUDE

#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

std::vector< std::string > ShaderSource::shaderIncludePaths;

ShaderSource::ShaderSource(void) : _sourceFile({"", 0}), _module(VK_NULL_HANDLE), _threadWidth(1), _threadHeight(1), _threadDepth(1)
{
}

ShaderSource::ShaderSource(const std::string & shaderName, VkShaderStageFlagBits stage) : ShaderSource()
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

#if __APPLE__
	return st.st_mtimespec.tv_sec;
#else
	return st.st_mtime;
#endif
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
	struct stat buffer;
	std::string	filePath = file;
	_stage = stage;

	// If the file don't exists, check inside include paths
	if (stat(file.c_str(), &buffer) != 0)
	{
		for (const auto & includePath : shaderIncludePaths)
		{
			std::string newPath = includePath + file;
			if (stat(newPath.c_str(), &buffer) == 0)
			{
				filePath = newPath;
				break ;
			}
		}
	}

	_sourceFile = ShaderFileInfo{filePath, GetFileModificationTime(file)};
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
}

void		ShaderSource::GenerateBindingTable(ShaderBindingTable & bindingTable)
{
	auto reflection = new spirv_cross::CompilerReflection(std::move(_SpirVCode));

	// Retrieve working group size of compute shader
	if (_stage == VK_SHADER_STAGE_COMPUTE_BIT)
	{
		const auto & entry = reflection->get_entry_point("main", spv::ExecutionModel::ExecutionModelGLCompute);

		_threadWidth = entry.workgroup_size.x;
		_threadHeight = entry.workgroup_size.y;
		_threadDepth = entry.workgroup_size.z;
	}

	spirv_cross::ShaderResources resources = reflection->get_shader_resources();

	// Warning: currently no differenciation between Buffer/Texture2D and RWBuffer/RWTexture2D is done
	const auto & addBinding = [&](const spirv_cross::Resource & resource, const VkDescriptorType descriptorType) {
		unsigned set = reflection->get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = reflection->get_decoration(resource.id, spv::DecorationBinding);
		const spirv_cross::SPIRType & type = reflection->get_type(resource.type_id);
		auto & shaderBinding = bindingTable.AddBinding(resource.name, set, binding, descriptorType);
		if (type.basetype == spirv_cross::SPIRType::Struct)
		{
			shaderBinding.elementSize = reflection->get_declared_struct_size(type);
		}
	};

	// Uniform buffers
	for (auto & resource : resources.uniform_buffers)
		addBinding(resource, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

	// SRVs
	for (auto & resource : resources.separate_images)
		addBinding(resource, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	// UAVs
	for (auto & resource : resources.storage_images)
		addBinding(resource, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);

	// Samplers
	for (auto & resource : resources.separate_samplers)
		addBinding(resource, VK_DESCRIPTOR_TYPE_SAMPLER);

	// StructedBuffers RW and read-only
	for (auto & resource : resources.storage_buffers)
		addBinding(resource, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

	delete reflection;
}

void		ShaderSource::GetWorkingThreadSize(uint32_t & width, uint32_t & height, uint32_t & depth) const
{
	width = _threadWidth;
	height = _threadHeight;
	depth = _threadDepth;
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