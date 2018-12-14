#include "ComputeDispatcher.hpp"

#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Hierarchy.hpp"

using namespace LWGC;

ComputeDispatcher::ComputeDispatcher(std::shared_ptr< Material > material, int width, int height, int depth) : 
	_material(material), _width(width), _height(height), _depth(depth)
{
}

ComputeDispatcher::~ComputeDispatcher(void)
{
	_material.reset();
}

void	ComputeDispatcher::RecordComputeCommand(VkCommandBuffer cmd) noexcept
{
	vkCmdDispatch(cmd, _width, _height, _depth);
}

void	ComputeDispatcher::CreateDescriptorSet(void)
{

}

void			ComputeDispatcher::Initialize(void) noexcept
{
	Component::Initialize();

	VulkanRenderPipeline * renderPipeline = VulkanRenderPipeline::Get();
	
	_material->Initialize(renderPipeline->GetSwapChain(), renderPipeline->GetRenderPass());
	_computeCommandBuffer = VulkanInstance::Get()->GetGraphicCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_SECONDARY);

	// Record command buffer:
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

	if (vkBeginCommandBuffer(_computeCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	vkCmdBindPipeline(_computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, _material->GetPipeline());

	// TODO: hardcoded !
	auto descriptorSet = _material->GetDescriptorSet();
	vkCmdBindDescriptorSets(_computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, _material->GetPipelineLayout(), 1, 1, &descriptorSet, 0, nullptr);

	vkCmdDispatch(_computeCommandBuffer, _width, _height, _depth);

	if (vkEndCommandBuffer(_computeCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}

	std::cout << "Completed !\n";
}

void			ComputeDispatcher::OnEnable() noexcept
{
	Component::OnEnable();
	_renderContextIndex = hierarchy->RegisterComponentInRenderContext(GetType(), this);
}

void			ComputeDispatcher::OnDisable() noexcept
{
	Component::OnDisable();
	hierarchy->UnregisterComponentInRenderContext(GetType(), _renderContextIndex);
}

std::shared_ptr< Material >	ComputeDispatcher::GetMaterial(void)
{
	return _material;
}

VkCommandBuffer				ComputeDispatcher::GetCommandBuffer(void)
{
	return _computeCommandBuffer;
}

uint32_t		ComputeDispatcher::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::ComputeDispatcher);
}

std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r)
{
	o << "ComputeDispatcher" << std::endl;
	(void)r;
	return (o);
}
