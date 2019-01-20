#pragma once

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE

#include <vector>

namespace LWGC
{
    class PipelineCommandBuffer
    {
        protected:
            std::vector< VkCommandBuffer >  _commandBuffers;

        public:
            PipelineCommandBuffer(void) = default;
            PipelineCommandBuffer(const PipelineCommandBuffer & rhs) = delete;
            virtual ~PipelineCommandBuffer(void) = default;

            PipelineCommandBuffer &     operator=(const PipelineCommandBuffer & rhs) = delete;

            virtual void                Allocate(size_t commandBufferCount);
			virtual void	            RecordCommands(VkCommandBuffer cmd) = 0;
            virtual VkCommandBuffer     GetCommandBuffer(size_t frameIndex) const;
    };
}