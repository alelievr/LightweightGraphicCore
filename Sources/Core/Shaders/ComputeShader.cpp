#include "ComputeShader.hpp"

using namespace LWGC;

ComputeShader::ComputeShader(void) : _material(nullptr)
{

}

ComputeShader::ComputeShader(const std::string & shaderPath)
{
	LoadShader(shaderPath);
}

void		ComputeShader::LoadShader(const std::string & shaderPath)
{
	_material = Material::Create(shaderPath, VK_SHADER_STAGE_COMPUTE_BIT);
	_dispatcher = ComputeDispatcher(_material, 64, 64, 64); // Default compute dispatch size
}

void		ComputeShader::Dispatch(VkCommandBuffer cmd, glm::ivec3 dispatchSize) noexcept
{
	_dispatcher.SetDispatchSize(dispatchSize);
	_dispatcher.RecordCommands(cmd);
}

ComputeShader &	ComputeShader::operator=(ComputeShader const & src)
{
	std::cout << "Assignment operator of ComputeShader called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	LWGC::operator<<(std::ostream & o, ComputeShader const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
