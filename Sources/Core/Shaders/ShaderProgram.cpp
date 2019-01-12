#include "ShaderProgram.hpp"

#include <algorithm>

#include "Core/Vulkan/Vk.hpp"
#include "Core/Application.hpp"

using namespace LWGC;

ShaderProgram::ShaderProgram(void)
{
}

ShaderProgram::ShaderProgram(const std::string & fragmentShaderName, const std::string & vertexShaderName)
{
	SetSourceFile(fragmentShaderName, VK_SHADER_STAGE_FRAGMENT_BIT);
	SetSourceFile(vertexShaderName, VK_SHADER_STAGE_VERTEX_BIT);
}

ShaderProgram::~ShaderProgram(void)
{
	Application::update.RemoveListener(_updateIndex);
}

void		ShaderProgram::CompileAndLink(void)
{
	_bindingTable.SetStage(IsCompute() ? VK_SHADER_STAGE_COMPUTE_BIT : VK_SHADER_STAGE_ALL_GRAPHICS);

	for (auto & shaderSource : _shaderSources)
	{
		shaderSource->Compile();
		shaderSource->GenerateBindingTable(_bindingTable);

		if (IsCompute())
			shaderSource->GetWorkingThreadSize(_threadWidth, _threadHeight, _threadDepth);

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shaderSource->GetStage();
		shaderStageInfo.module = shaderSource->GetModule();
		shaderStageInfo.pName = "main";

		_shaderStages.push_back(shaderStageInfo);
	}

	_bindingTable.GenerateSetLayouts();
	_updateIndex = Application::update.AddListener(std::bind(&ShaderProgram::Update, this));
}

bool		ShaderProgram::IsCompiled(void) const noexcept
{
	return _shaderStages.size() > 0;
}

void		ShaderProgram::SetSourceFile(const std::string & file, VkShaderStageFlagBits stage)
{
	if (IsCompiled())
		throw std::runtime_error("Can't add shader source file after it's compiled: " + file);

	_shaderSources.push_back(new ShaderSource(file, stage));
}

bool		ShaderProgram::IsCompute(void) const noexcept
{
	return std::any_of(_shaderSources.begin(), _shaderSources.end(), [](const auto & s){
		return s->GetStage() == VK_SHADER_STAGE_COMPUTE_BIT;
	});
}

void		ShaderProgram::Update(void)
{
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
			Application::Get()->GetMaterialTable()->UpdateMaterial(this);
		}
	}
}

VkPipelineShaderStageCreateInfo *		ShaderProgram::GetShaderStages(void)
{
	return _shaderStages.data();
}

void			ShaderProgram::GetWorkingThreadSize(uint32_t & width, uint32_t & height, uint32_t & depth)
{
	width = _threadWidth;
	height = _threadHeight;
	depth = _threadDepth;
}

bool			ShaderProgram::HasBinding(const std::string & bindingName) const
{
	return _bindingTable.HasBinding(bindingName);
}

const ShaderBindingTable *	ShaderProgram::GetShaderBindingTable(void) const
{
	return &_bindingTable;
}

std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
