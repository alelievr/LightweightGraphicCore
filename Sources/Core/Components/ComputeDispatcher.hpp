#pragma once

#include "IncludeDeps.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Textures/Texture2D.hpp"

#include <iostream>
#include <string>
#include <memory>

namespace LWGC
{
	class		ComputeDispatcher : public Component
	{
		friend class ComputeShader;

		using MemoryBarrierInfo = std::pair< VkMemoryBarrier, VkPipelineStageFlags >;
		using BufferBarrierInfo = std::pair< VkBufferMemoryBarrier, VkPipelineStageFlags >;
		using ImageBarrierInfo = std::pair< VkImageMemoryBarrier, VkPipelineStageFlags >;

		private:
			Material *						_material;
			int								_width;
			int								_height;
			int								_depth;
			uint32_t						_workGroupWidth;
			uint32_t						_workGroupHeight;
			uint32_t						_workGroupDepth;
			VkCommandBuffer					_computeCommandBuffer;
			ComponentIndex					_renderContextIndex;
			std::vector< MemoryBarrierInfo >_memoryBarriers;
			std::vector< BufferBarrierInfo >_bufferBarriers;
			std::vector< ImageBarrierInfo >	_imageBarriers;

			virtual void	Initialize(void) noexcept override;
			void			OnEnable(void) noexcept override;
			void			OnDisable(void) noexcept override;

			bool			CheckWorkSize(void) noexcept;

		public:
			ComputeDispatcher(void) = delete;
			ComputeDispatcher(Material * material, int width, int height, int depth = 1);
			ComputeDispatcher(const ComputeDispatcher &) = delete;
			virtual ~ComputeDispatcher(void);

			ComputeDispatcher &	operator=(ComputeDispatcher const & src) = delete;

			void				RecordCommands(VkCommandBuffer cmd);

			void				SetDispatchSize(const glm::ivec3 & size, bool checkSize = true);

			void				AddMemoryBarrier(VkMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask);
			void				AddBufferBarrier(VkBufferMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask);
			void				AddImageBarrier(VkImageMemoryBarrier barrier, VkPipelineStageFlags destinationStageMask);

			Material *			GetMaterial(void);
			VkCommandBuffer		GetCommandBuffer(void);

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r);
}
