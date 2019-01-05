#pragma once

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE

namespace LWGC
{
    class IPipelineCommandBuffer
    {
        public:
			virtual void	            RecordCommands(VkCommandBuffer cmd) = 0;
            virtual VkCommandBuffer     GetCommandBuffer(void) const = 0;
    };
}