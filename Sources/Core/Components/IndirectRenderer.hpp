#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Component.hpp"
#include "Core/Components/Renderer.hpp"

namespace LWGC
{
	class		IndirectRenderer : public Renderer
	{
		private:
			Bounds		_bounds;
			uint32_t	_drawCount;
			uint32_t	_stride;
			uint32_t	_offset;
			VkBuffer	_drawBuffer;

			void		RecordDrawCommand(VkCommandBuffer cmd) noexcept override;

		public:
			IndirectRenderer(void) = delete;
			IndirectRenderer(const IndirectRenderer &) = delete;
			IndirectRenderer(Material * material);
			virtual ~IndirectRenderer(void);

			IndirectRenderer &	operator=(IndirectRenderer const & src) = delete;

			Bounds	GetBounds(void) noexcept override;
			void	SetBounds(const Bounds & bound) noexcept;

			void	SetDrawCount(uint32_t drawCount, uint32_t stride, uint32_t offset = 0) noexcept;
			void	SetDrawBuffer(VkBuffer drawBuffer) noexcept;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, IndirectRenderer const & r);
}
