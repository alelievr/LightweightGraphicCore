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

void	ForwardRenderPipeline::Render(void)
{
	for (int i = 0; i < static_cast< int >(RenderQueueType::Count); i++)
		this->_renderQueues[i].RenderPass();
}

void	ForwardRenderPipeline::RenderImGUI(void)
{
	bool test;

	ImGui::Begin("Hello, world!");
	ImGui::Checkbox("Test", &test);
	ImGui::End();
}

void	ForwardRenderPipeline::SetRenderTarget(RenderTarget & target)
{
	this->_target = target;
}


void	ForwardRenderPipeline::PushToQueue(std::shared_ptr< IRenderable > renderable, RenderQueueType queueType)
{
	_renderQueues[(int)queueType].AddToRender(renderable);
}
