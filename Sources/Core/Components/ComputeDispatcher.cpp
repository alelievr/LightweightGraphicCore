#include "ComputeDispatcher.hpp"

#include "Core/Rendering/RenderPipeline.hpp"
#include "Core/Hierarchy.hpp"

using namespace LWGC;

ComputeDispatcher::ComputeDispatcher(Material * material, int width, int height, int depth) :
	_material(material), _width(width), _height(height), _depth(depth), _workGroupWidth(1), _workGroupHeight(1), _workGroupDepth(1)
{
}

ComputeDispatcher::~ComputeDispatcher(void)
{

}

void			ComputeDispatcher::Initialize(void) noexcept
{
	Component::Initialize();

	_material->MarkAsReady();

	_material->GetComputeWorkSize(_workGroupWidth, _workGroupHeight, _workGroupDepth);

	if (!CheckWorkSize())
	{
		std::cout << "Dispatch size for compute shader is not multiple of his work group size: "
			<< _workGroupWidth << ", " << _workGroupHeight << ", " << _workGroupDepth << std::endl;
		return ;
	}
}

bool			ComputeDispatcher::CheckWorkSize(void) noexcept
{
	return (_width % _workGroupWidth != 0 || _height % _workGroupHeight != 0 || _depth % _workGroupDepth != 0);
}

void			ComputeDispatcher::RecordCommands(VkCommandBuffer cmd)
{
	vkCmdDispatch(cmd, _width / _workGroupWidth, _height / _workGroupHeight, _depth / _workGroupDepth);

	// Image barriers
	for (const auto & imageBarrierInfo : _imageBarriers)
	{
		vkCmdPipelineBarrier(
			cmd,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			imageBarrierInfo.second,
			0,										// Dependency flags
			0, nullptr,
			0, nullptr,
			1, &imageBarrierInfo.first              // pImageMemoryBarriers
		);
	}
	
	// Memory barriers
	for (const auto & memoryBarrierInfo : _memoryBarriers)
	{
		vkCmdPipelineBarrier(
			cmd,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			memoryBarrierInfo.second,
			0,										// Dependency flags
			1, &memoryBarrierInfo.first,
			0, nullptr,
			0, nullptr
		);
	}
	
	// Buffer barriers
	for (const auto & bufferBarrierInfo : _bufferBarriers)
	{
		vkCmdPipelineBarrier(
			cmd,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			bufferBarrierInfo.second,
			0,										// Dependency flags
			0, nullptr,
			1, &bufferBarrierInfo.first,
			0, nullptr
		);
	}
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

void			ComputeDispatcher::SetDispatchSize(const glm::ivec3 & size, bool checkSize)
{
	_width = size.x;
	_height = size.y;
	_depth = size.z;

	if (checkSize && !CheckWorkSize())
	{
		std::cout << "Dispatch size for compute shader is not multiple of his work group size: "
			<< _workGroupWidth << ", " << _workGroupHeight << ", " << _workGroupDepth << std::endl;
	}
}

void			ComputeDispatcher::AddMemoryBarrier(VkMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_memoryBarriers.push_back({barrier, destinationStageMask});
}

void			ComputeDispatcher::AddBufferBarrier(VkBufferMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_bufferBarriers.push_back({barrier, destinationStageMask});
}

void			ComputeDispatcher::AddImageBarrier(VkImageMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask)
{
	_imageBarriers.push_back({barrier, destinationStageMask});
}


Material *		ComputeDispatcher::GetMaterial(void)
{
	return _material;
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
