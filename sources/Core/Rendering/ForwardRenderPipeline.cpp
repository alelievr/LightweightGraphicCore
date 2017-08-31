#include "ForwardRenderPipeline.hpp"


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

void	ForwardRenderPipeline::SetRenderTarget(RenderTarget & traget)
{
	this->_target = _target;
}
