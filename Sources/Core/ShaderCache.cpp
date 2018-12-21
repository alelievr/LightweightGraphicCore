#include "ShaderCache.hpp"

using namespace LWGC;

std::unordered_map<std::string, ShaderProgram *> ShaderCache::_shaders;

ShaderProgram		*ShaderCache::GetShader(const std::string & shader, const std::string & elem)
{
	if (_shaders.find(shader) != _shaders.end())
	{
		ShaderProgram	 *program = new ShaderProgram();
		_shaders[shader + elem] = program;
		program->SetSourceFile(shader, VK_SHADER_STAGE_FRAGMENT_BIT);
		program->SetSourceFile(elem, VK_SHADER_STAGE_VERTEX_BIT);
		return (program);
	}
	return (_shaders[shader + elem]);
}

ShaderProgram		*ShaderCache::GetShader(const std::string & shader, int elem)
{
	if (_shaders.find(shader) != _shaders.end())
	{
		ShaderProgram	 *program = new ShaderProgram();
		_shaders[shader + std::to_string(elem)] = program;
		program->SetSourceFile(shader, VK_SHADER_STAGE_FRAGMENT_BIT);
		program->SetSourceFile(std::to_string(elem), VK_SHADER_STAGE_VERTEX_BIT);
		return (program);
	}
	return (_shaders[shader + std::to_string(elem)]);
}

