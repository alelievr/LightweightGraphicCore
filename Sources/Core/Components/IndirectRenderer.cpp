#include "IndirectRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/RenderPipeline.hpp"

using namespace LWGC;

IndirectRenderer::IndirectRenderer(Material * material)
	: Renderer(material), _drawCount(1), _stride(0), _offset(0), _bufferCount(0)
{
	_instance = VulkanInstance::Get();

	// Make sure to bind the same vertex layout than the format of the draw buffer for the material
}

IndirectRenderer::~IndirectRenderer(void)
{
}

void		IndirectRenderer::RecordDrawCommand(VkCommandBuffer cmd, uint32_t frameIndex) noexcept
{
	vkCmdDrawIndirect(cmd, _drawBuffers[frameIndex], _offset, _drawCount, _stride);
}

void		IndirectRenderer::RecordDrawCommand(VkCommandBuffer cmd) noexcept
{
	std::cerr << "For indirectRenderer, please use the override of RecordDrawCommand with the frameIndex in parameter" << std::endl;
}

uint32_t					IndirectRenderer::GetType(void) const noexcept { return static_cast< uint32_t >(ComponentType::IndirectRenderer); }

Bounds	IndirectRenderer::GetBounds(void) noexcept { return _bounds; }

void	IndirectRenderer::SetBounds(const Bounds & bounds) noexcept { _bounds = bounds; }

void	IndirectRenderer::SetOffset(uint32_t offset) noexcept { _offset = offset; }

void	IndirectRenderer::SetDrawCount(uint32_t drawCount, uint32_t stride, uint32_t offset) noexcept
{
	_drawCount = drawCount;
	_stride = stride;
	_offset = offset;
}

void		IndirectRenderer::AllocateDrawBuffer(VkBufferUsageFlags bufferUsage, uint32_t swapchainImageCount, uint32_t bufferCount) noexcept
{
	_bufferCount = bufferCount;
	_swapchainImageCount = swapchainImageCount;

	_drawBuffers.resize(swapchainImageCount);
	_drawMemories.resize(swapchainImageCount);

	// Allocate one draw buffer per swapchain so it does not causes binding issue when we generate the next frame
	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{
		Vk::CreateBuffer(sizeof(VkDrawIndirectCommand) * bufferCount, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | bufferUsage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, _drawBuffers[i], _drawMemories[i]);

		// Initialize the indirect draw buffer to 0 to avoid your computer to explode if you attempt to draw with a uninitialized buffer
		for (uint32_t j = 0; j < bufferCount; j++)
			ResetDrawBuffer(i, j);
	}
}

VkBuffer	IndirectRenderer::GetDrawBuffer(size_t frameIndex, size_t & bufferIndex) noexcept
{
	// Compute the offset within the allocated buffer to use:
	bufferIndex = frameIndex * _bufferCount;
	return _drawBuffers[frameIndex];
}

void		IndirectRenderer::ResetDrawBuffer(size_t frameIndex, size_t bufferIndex) noexcept
{
	SetDrawBufferValues(frameIndex, bufferIndex, 0, 0, 0, 0);
}

void		IndirectRenderer::SetDrawBufferValues(size_t frameIndex, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) noexcept
{
	SetDrawBufferValues(frameIndex, 0, vertexCount, instanceCount, firstVertex, firstInstance);
}

uint32_t	IndirectRenderer::GetDrawBufferSize(void) const noexcept
{
	return _bufferCount * sizeof(VkDrawIndirectCommand);
}

void		IndirectRenderer::SetDrawBufferValues(size_t frameIndex, size_t bufferIndex, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) noexcept
{
	VkDrawIndirectCommand drawCommand = {};
	drawCommand.vertexCount = vertexCount;
	drawCommand.instanceCount = instanceCount;
	drawCommand.firstVertex = firstVertex;
	drawCommand.firstInstance = firstInstance;

	// Reset the draw buffer at each frame
	Vk::UploadToMemory(_drawMemories[frameIndex], &drawCommand, sizeof(VkDrawIndirectCommand), sizeof(VkDrawIndirectCommand) * bufferIndex);
}

std::ostream &	operator<<(std::ostream & o, IndirectRenderer const & r)
{
	o << "IndirectRenderer" << std::endl;
	(void)r;
	return (o);
}
