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
	_renderPass.Initialize(nullptr);
	_dispatcher._material = _material;
}

void		ComputeShader::Dispatch(VkCommandBuffer cmd, int width, int height, int depth) noexcept
{
	_renderPass.Begin(cmd, VK_NULL_HANDLE, "TODO: Dispatch compute name");
	{
		_renderPass.BindMaterial(_material);
		_material->BindPipeline(cmd);
		_material->BindProperties(cmd);
		_renderPass.UpdateDescriptorBindings();
		_dispatcher.SetDispatchSize(glm::ivec3(width, height, depth));
		_dispatcher.RecordCommands(cmd);
	}
	_renderPass.End();
}

void		ComputeShader::SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType, bool silent)
{
	_material->SetBuffer(bindingName, buffer, size, descriptorType, silent);
}

void		ComputeShader::SetTexture(const std::string & bindingName, const Texture * texture, VkImageLayout imageLayout, VkDescriptorType descriptorType, bool silent)
{
	_material->SetTexture(bindingName, texture, imageLayout, descriptorType, silent);
}

void		ComputeShader::SetSampler(const std::string & bindingName, VkSampler sampler, bool silent)
{
	_material->SetSampler(bindingName, sampler, silent);
}

void		ComputeShader::AddMemoryBarrier(VkMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_dispatcher.AddMemoryBarrier(barrier, destinationStageMask);
}

void		ComputeShader::AddBufferBarrier(VkBufferMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_dispatcher.AddBufferBarrier(barrier, destinationStageMask);
}

void		ComputeShader::AddImageBarrier(VkImageMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_dispatcher.AddImageBarrier(barrier, destinationStageMask);
}

std::ostream &	LWGC::operator<<(std::ostream & o, ComputeShader const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
