#include "ForwardRenderPipeline.hpp"

using namespace LWGC;

ForwardRenderPipeline::ForwardRenderPipeline(void)
{
}

ForwardRenderPipeline::~ForwardRenderPipeline(void)
{
}

void	ForwardRenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext * context)
{
	RenderPipeline::Render(cameras, context);
}