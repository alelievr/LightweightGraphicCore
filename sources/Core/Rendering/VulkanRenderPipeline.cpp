#include "VulkanRenderPipeline.hpp"

using namespace LWGC;

VulkanRenderPipeline::VulkanRenderPipeline(void)
{

}

VulkanRenderPipeline::~VulkanRenderPipeline(void)
{
    
}

void                VulkanRenderPipeline::Initialize(SwapChain * swapChain)
{
    instance = VulkanInstance::Get();
    swapChain = swapChain;
	renderPass.Initialize(swapChain);
    material.Initialize(swapChain, &renderPass);
    graphicCommandBufferPool = instance->GetGraphicCommandBufferPool();
}

void				VulkanRenderPipeline::CreateRenderPass(void)
{
	// Currently fixed renderpass, TODO: have the hability to change the layout of the renderPass
	renderPass.AddAttachment(
		RenderPass::GetDefaultColorAttachment(swapChain->GetImageFormat()),
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	renderPass.SetDepthAttachment(
		RenderPass::GetDefaultDepthAttachment(instance->FindDepthFormat()),
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPass.AddDependency(dependency);

	// Will also generate framebuffers
	renderPass.Create();
}

void                VulkanRenderPipeline::CreateMaterials()
{
    material.Initialize(swapChain, &renderPass);
}