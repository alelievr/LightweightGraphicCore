#include "ShaderProgram.hpp"

#include <algorithm>

using namespace LWGC;

ShaderProgram *	ShaderProgram::Standard = new ShaderProgram("Shaders/Debug/AlbedoTexture.hlsl");

ShaderProgram::ShaderProgram(void)
{
}

ShaderProgram::ShaderProgram(const std::string & fragmentShaderName, const std::string & vertexShaderName)
{
	SetFragmentSourceFile(fragmentShaderName);
	SetVertexSourceFile(vertexShaderName);
}

ShaderProgram::~ShaderProgram(void)
{
	std::cout << "Destructor of ShaderProgram called" << std::endl;
}

void		ShaderProgram::CompileAndLink(void)
{
	for (auto & shaderSource : _shaderSources)
	{
		shaderSource->Compile();

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shaderSource->GetStage();
		shaderStageInfo.module = shaderSource->GetModule();
		shaderStageInfo.pName = "main";

		_shaderStages.push_back(shaderStageInfo);
	}
}

bool		ShaderProgram::IsCompiled(void) const noexcept
{
	return _shaderStages.size() > 0;
}

void		ShaderProgram::SetVertexSourceFile(const std::string & file)
{
	_vertexShaderSource.SetSourceFile(file, VK_SHADER_STAGE_VERTEX_BIT);
	_shaderSources.push_back(&_vertexShaderSource);
}

void		ShaderProgram::SetFragmentSourceFile(const std::string & file)
{
	_fragmentShaderSource.SetSourceFile(file, VK_SHADER_STAGE_FRAGMENT_BIT);
	_shaderSources.push_back(&_fragmentShaderSource);
}

void		ShaderProgram::SetGeometrySourceFile(const std::string & file)
{
	_geometryShaderSource.SetSourceFile(file, VK_SHADER_STAGE_GEOMETRY_BIT);
	_shaderSources.push_back(&_geometryShaderSource);
}

void		ShaderProgram::SetComputeSourceFile(const std::string & file)
{
	_computeShaderSource.SetSourceFile(file, VK_SHADER_STAGE_COMPUTE_BIT);
	_shaderSources.push_back(&_computeShaderSource);
}

bool		ShaderProgram::IsCompute(void) const noexcept
{
	return _computeShaderSource.HasSource();
}

bool		ShaderProgram::Update(void)
{
	bool		hasReloaded = false;

	for (auto & shaderSource : _shaderSources)
	{
		if (shaderSource->NeedReload())
		{
			shaderSource->Reload();

			// Update stage code:
			const auto & t = std::find_if(_shaderStages.begin(), _shaderStages.end(),
				[& shaderSource](const VkPipelineShaderStageCreateInfo & s) {
				return shaderSource->GetStage() == s.stage;
			});

			if (t == _shaderStages.end())
				throw std::runtime_error("Failed to reload shader program code, you're probably adding shader sources after the program being compiled and linked");

			t->module = shaderSource->GetModule();

			hasReloaded = true;
		}
	}

	return hasReloaded;
}

VkPipelineShaderStageCreateInfo *	ShaderProgram::GetShaderStages()
{
	return _shaderStages.data();
}

std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
