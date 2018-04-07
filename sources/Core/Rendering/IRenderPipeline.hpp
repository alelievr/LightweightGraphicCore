#pragma once

#include "RenderTarget.hpp"
#include "RenderQueueType.hpp"
#include "../Model.hpp"

namespace LWGE
{
	class IRenderPipeline
	{
		public:
			virtual void	Render(void) = 0;
			virtual void	SetRenderTarget(RenderTarget & target) = 0;

			virtual void	PushToQueue(const Model &, RenderQueueType queueType) = 0;
	};
}
