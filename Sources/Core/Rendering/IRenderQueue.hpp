#pragma once

#include <vector>

#include "Core/Components/Renderer.hpp"

namespace LWGC
{
	class IRenderQueue
	{
		public:
			virtual ~IRenderQueue(void) = default;

			virtual void						AddRenderer(Renderer * renderer) noexcept = 0;

			// Getters
			virtual uint32_t					GetQueueCount(void) noexcept = 0;
			virtual std::vector< Renderer * >	GetRenderersForQueue(uint32_t queueIndex) noexcept = 0;
	};
}
