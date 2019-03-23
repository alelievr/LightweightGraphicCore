#include "ShaderProgram.hpp"

#include <algorithm>

#include "Core/Vulkan/Vk.hpp"
#include "Core/Application.hpp"

using namespace LWGC;

ShaderProgram::ShaderProgram(void) : _threadWidth(1), _threadHeight(1), _threadDepth(1), _isUpdateBound(false)
{
}

ShaderProgram::ShaderProgram(const std::string & fragmentShaderName, const std::string & vertexShaderName) : ShaderProgram()
{
	SetSourceFile(fragmentShaderName, VK_SHADER_STAGE_FRAGMENT_BIT);
	SetSourceFile(vertexShaderName, VK_SHADER_STAGE_VERTEX_BIT);

	_name = GetFileName(fragmentShaderName);
}

ShaderProgram::~ShaderProgram(void)
{
	Application::update.RemoveListener(_updateIndex);
}

void		ShaderProgram::CompileAndLink(void)
{
	_bindingTable.SetStage(IsCompute() ? VK_SHADER_STAGE_COMPUTE_BIT : VK_SHADER_STAGE_ALL_GRAPHICS);
	if (!_isUpdateBound)
	{
		_updateIndex = Application::update.AddListener(std::bind(&ShaderProgram::Update, this));
		_isUpdateBound = true;
	}

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
}

bool		ShaderProgram::IsCompiled(void) const noexcept
{
	return _shaderStages.size() > 0;
}

void		ShaderProgram::SetSourceFile(const std::string & file, VkShaderStageFlagBits stage)
{
	if (IsCompiled())
		throw std::runtime_error("Can't add shader source file after it's compiled: " + file);

	if (_name.empty())
		_name = GetFileName(file);

	_shaderSources.push_back(new ShaderSource(file, stage));
}

const std::string	ShaderProgram::GetFileName(const std::string & filePath)
{
	std::string fileName = filePath;
	const size_t last_slash_idx = filePath.find_last_of("\\/");

	if (std::string::npos != last_slash_idx)
	{
		fileName.erase(0, last_slash_idx + 1);
	}
	return fileName;
}

bool		ShaderProgram::IsCompute(void) const noexcept
{
	return std::any_of(_shaderSources.begin(), _shaderSources.end(), [](const auto & s){
		return s->GetStage() == VK_SHADER_STAGE_COMPUTE_BIT;
	});
}

void		ShaderProgram::Update(void)
{
	auto device = VulkanInstance::Get()->GetDevice();

	for (auto & shaderSource : _shaderSources)
	{
		if (shaderSource->NeedReload())
		{
			for (auto stage : _shaderStages)
				vkDestroyShaderModule(device, stage.module, nullptr);
			_shaderStages.clear();
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

const std::string		ShaderProgram::GetName(void) const
{
	return _name;
}

std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
