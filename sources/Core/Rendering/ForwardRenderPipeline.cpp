#include "ForwardRenderPipeline.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
// TODO: move to vulkan
#include "imgui_impl_opengl3.h"

using namespace LWGC;

ForwardRenderPipeline::ForwardRenderPipeline(void)
{
}

ForwardRenderPipeline::~ForwardRenderPipeline(void)
{
}

void	ForwardRenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext & context)
{
	VulkanRenderPipeline::Render(cameras, context);
}