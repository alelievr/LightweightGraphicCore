#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Component.hpp"
#include "Core/Components/Renderer.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

namespace LWGC
{
	class		IndirectRenderer : public Renderer
	{
		private:
			Bounds			_bounds;
			uint32_t		_drawCount;
			uint32_t		_stride;
			uint32_t		_offset;
			std::vector< VkBuffer >			_drawBuffers;
			std::vector< VkDeviceMemory >	_drawMemories;
			uint32_t		_bufferCount;
			VulkanInstance *_instance;
			uint32_t		_swapchainImageCount;

			void		RecordDrawCommand(VkCommandBuffer cmd) noexcept override;

		public:
			IndirectRenderer(void) = delete;
			IndirectRenderer(const IndirectRenderer &) = delete;
			IndirectRenderer(Material * material);
			virtual ~IndirectRenderer(void);

			IndirectRenderer &	operator=(IndirectRenderer const & src) = delete;
			
			void		AllocateDrawBuffer(VkBufferUsageFlags bufferUsage, uint32_t swapchainImageCount, uint32_t bufferCount = 1) noexcept;
			void		ResetDrawBuffer(size_t frameIndex, size_t bufferIndex) noexcept;

			Bounds	GetBounds(void) noexcept override;
			void	SetBounds(const Bounds & bound) noexcept;

			void		SetDrawCount(uint32_t drawCount, uint32_t stride, uint32_t offset = 0) noexcept;
			void		SetOffset(uint32_t offset) noexcept;
			void		SetDrawBufferValues(size_t frameIndex, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) noexcept;
			void		SetDrawBufferValues(size_t frameIndex, size_t bufferIndex, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) noexcept;
			VkBuffer	GetDrawBuffer(size_t frameIndex, size_t & bufferIndex) noexcept;
			uint32_t	GetDrawBufferSize(void) const noexcept;
			
			void		RecordDrawCommand(VkCommandBuffer cmd, uint32_t frameIndex) noexcept;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, IndirectRenderer const & r);
}
