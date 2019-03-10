#include "ForwardRenderPipeline.hpp"
#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

void	ForwardRenderPipeline::Initialize(SwapChain * swapChain)
{
	RenderPipeline::Initialize(swapChain);

	heavyComputeShader.LoadShader("Shaders/Compute/Heavy.hlsl");
	heavyComputeFence = Vk::CreateFence(true);

	// Allocate an async command queue (the device must have more than one queue to run the application)
	instance->AllocateDeviceQueue(asyncComputeQueue, asyncComputeQueueIndex);
	asyncComputePool.Initialize(asyncComputeQueue, asyncComputeQueueIndex);

	// Setup the render passes we uses:
	SetupRenderPasses();
}

void	ForwardRenderPipeline::SetupRenderPasses()
{
	// Compute pass (for each frame)
	computePass.Initialize();
	// We don't have any attachements for the compute pass
	computePass.Create();

	// Forward pass (render all objects into the framebuffer)
	forwardPass.Initialize();

	forwardPass.AddAttachment(
		RenderPass::GetDefaultColorAttachment(swapChain->GetImageFormat()),
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	);
	forwardPass.SetDepthAttachment(
		RenderPass::GetDefaultDepthAttachment(instance->FindDepthFormat()),
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	);

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT; // useful ?

	forwardPass.AddDependency(dependency);

	forwardPass.Create();
	swapChain->CreateFrameBuffers(forwardPass); // Must be the last renderpass before the final blit (which contains the framebuffer attachements)
}

void	ForwardRenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext * context)
{
	VkCommandBuffer cmd = GetCurrentFrameCommandBuffer();

	if (vkGetFenceStatus(device, heavyComputeFence) == VK_SUCCESS)
	{
		std::cout << "Finished the compute heavy task, running another one !" << std::endl;

		auto computeCmd = asyncComputePool.BeginSingle();
		heavyComputeShader.Dispatch(computeCmd, 1024, 1024, 1);
		asyncComputePool.EndSingle(computeCmd, heavyComputeFence);
	}

	computePass.Begin();
	RenderPipeline::RecordAllComputeDispatches(cmd);
	computePass.End();

	forwardPass.Begin();
	RenderPipeline::RecordAllMeshRenderers(cmd);

	// Process the compute shader before everything:
	RenderPipeline::
	RenderPipeline::Render(cameras, context);
}