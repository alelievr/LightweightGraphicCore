#include "ShaderCache.hpp"

using namespace LWGC;

std::unordered_map<std::string, ShaderProgram *> ShaderCache::_shaders;

ShaderProgram		*ShaderCache::GetShader(const std::string & shader, const std::string & elem)
{
	std::string key = shader + elem;
	std::cout << "Added shader " << shader << std::endl;	

	// TODO: _shaders do not keep track of vertex shaders
	if (_shaders.find(key) == _shaders.end())
	{
		ShaderProgram	 *program = new ShaderProgram();
		program->SetSourceFile(shader, VK_SHADER_STAGE_FRAGMENT_BIT);
		program->SetSourceFile(elem, VK_SHADER_STAGE_VERTEX_BIT);

		_shaders[key] = program;

		return (program);
	}

	return (_shaders[key]);
}

ShaderProgram		*ShaderCache::GetShader(const std::string & shader, VkShaderStageFlagBits stage)
{
	std::string key = shader + std::to_string(stage);

	std::cout << "Added shader " << shader << std::endl;
	
	if (_shaders.find(key) == _shaders.end())
	{
		ShaderProgram	 *program = new ShaderProgram();
		program->SetSourceFile(shader, stage);
		_shaders[key] = program;

		return (program);
	}

	return (_shaders[key]);
}