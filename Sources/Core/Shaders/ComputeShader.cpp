#include "ComputeShader.hpp"

using namespace LWGC;

ComputeShader::ComputeShader(void) : _material(nullptr), _dispatcher(nullptr, 1, 1, 1)
{

}

ComputeShader::ComputeShader(const std::string & shaderPath) : ComputeShader()
{
	LoadShader(shaderPath);
}

void		ComputeShader::LoadShader(const std::string & shaderPath)
{
	_material = Material::Create(shaderPath, VK_SHADER_STAGE_COMPUTE_BIT);
	_dispatcher._material = _material;
}

void		ComputeShader::Dispatch(VkCommandBuffer cmd, int width, int height, int depth) noexcept
{
	_dispatcher.SetDispatchSize(glm::ivec3(width, height, depth));
	_dispatcher.RecordCommands(cmd);
}

std::ostream &	LWGC::operator<<(std::ostream & o, ComputeShader const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
