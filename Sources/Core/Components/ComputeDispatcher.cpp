#include "ComputeDispatcher.hpp"

#include "Core/Rendering/VulkanRenderPipeline.hpp"

using namespace LWGC;

ComputeDispatcher::ComputeDispatcher(std::shared_ptr< Material > material, int width, int height, int depth) : 
	_material(material), _width(width), _height(height), _depth(depth)
{
}

ComputeDispatcher::~ComputeDispatcher(void)
{
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
