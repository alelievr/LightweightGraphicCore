#pragma once

#include <iostream>
#include <string>

#include "VulkanRenderPipeline.hpp"
#include "RenderQueueType.hpp"
#include "RenderQueue.hpp"

namespace LWGC
{
	class		ForwardRenderPipeline : public VulkanRenderPipeline
	{
		private:
			RenderQueue		_renderQueues[static_cast< int >(RenderQueueType::Count)];
			RenderTarget	_target;

		public:
			ForwardRenderPipeline(void);
			virtual ~ForwardRenderPipeline(void);

			void	Render(void) override;
			void	RenderImGUI(void) override;
			void	SetRenderTarget(RenderTarget & target) override;
			void	PushToQueue(std::shared_ptr< IRenderable > renderable, RenderQueueType queueType) override;
	};
}
