#include "IndirectRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/RenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

IndirectRenderer::IndirectRenderer(Material * material)
	: Renderer(material), _drawCount(1), _stride(0), _drawBuffer(VK_NULL_HANDLE)
{
	// For procedural materials we don't need vertex input datas
	VkPipelineVertexInputStateCreateInfo	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	material->SetVertexInputState(vertexInputInfo);
}

IndirectRenderer::~IndirectRenderer(void)
{
}

void		IndirectRenderer::RecordDrawCommand(VkCommandBuffer cmd) noexcept
{
	if (_drawBuffer == VK_NULL_HANDLE)
	{
		std::cerr << "DrawIndirect without setting a valid draw buffer" << std::endl;
		return ;
	}

	vkCmdDrawIndirect(cmd, _drawBuffer, _offset, _drawCount, _stride);
}

uint32_t					IndirectRenderer::GetType(void) const noexcept { return static_cast< uint32_t >(ComponentType::IndirectRenderer); }

Bounds	IndirectRenderer::GetBounds(void) noexcept { return _bounds; }

void	IndirectRenderer::SetBounds(const Bounds & bounds) noexcept { _bounds = bounds; }

void	IndirectRenderer::SetDrawCount(uint32_t drawCount, uint32_t stride, uint32_t offset) noexcept
{
	_drawCount = drawCount;
	_stride = stride;
	_offset = offset;
}

void	IndirectRenderer::SetDrawBuffer(VkBuffer drawBuffer) noexcept { _drawBuffer = drawBuffer; }

std::ostream &	operator<<(std::ostream & o, IndirectRenderer const & r)
{
	o << "IndirectRenderer" << std::endl;
	(void)r;
	return (o);
}
