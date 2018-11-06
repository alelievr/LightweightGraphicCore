#pragma once

#include "RenderTarget.hpp"
#include "RenderQueueType.hpp"
#include "IRenderable.hpp"

namespace LWGC
{
	class IRenderPipeline
	{
		public:
			virtual void	Render(void) = 0;
			virtual void	RenderImGUI(void) = 0;
			virtual void	SetRenderTarget(RenderTarget & target) = 0;

			virtual void	PushToQueue(std::shared_ptr< IRenderable > renderable, RenderQueueType queueType) = 0;
	};
}
