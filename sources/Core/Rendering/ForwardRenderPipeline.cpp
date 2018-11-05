#include "ForwardRenderPipeline.hpp"

using namespace LWGC;

ForwardRenderPipeline::ForwardRenderPipeline(void)
{
}

ForwardRenderPipeline::~ForwardRenderPipeline(void)
{
}

void	ForwardRenderPipeline::Render(void)
{
	for (int i = 0; i < static_cast< int >(RenderQueueType::Count); i++)
		this->_renderQueues[i].RenderPass();
}

void	ForwardRenderPipeline::SetRenderTarget(RenderTarget & target)
{
	this->_target = target;
}


void	ForwardRenderPipeline::PushToQueue(std::shared_ptr< IRenderable > renderable, RenderQueueType queueType)
{
	_renderQueues[(int)queueType].AddToRender(renderable);
}
