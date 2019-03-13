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
	computePass.Initialize(swapChain);
	// We don't have any attachements for the compute pass
	computePass.Create();

	// Forward pass (render all objects into the framebuffer)
	forwardPass.Initialize(swapChain);

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

	forwardPass.SetClearColor(Color::Black, 1.0f, 0.0f);

	forwardPass.Create();

	// Must be the last renderpass before the final blit (which contains the framebuffer attachements)
	SetLastRenderPass(forwardPass);
}

void	ForwardRenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext * context)
{
	VkCommandBuffer cmd = GetCurrentFrameCommandBuffer();

	if (vkGetFenceStatus(device, heavyComputeFence) == VK_SUCCESS)
	{
		// std::cout << "Finished the compute heavy task, running another one !" << std::endl;

		// TODO: make this work
		// auto computeCmd = asyncComputePool.BeginSingle();
		// printf("computeCMd: %p\n", computeCmd);
		// heavyComputeShader.Dispatch(computeCmd, 1024, 1024, 1);
		// asyncComputePool.EndSingle(computeCmd, heavyComputeFence);
	}



	// Process the compute shader before everything:
	computePass.Begin(cmd, VK_NULL_HANDLE, "All Computes");
	computePass.BindDescriptorSet(LWGCBinding::Frame, perFrameDescriptorSet);
	RenderPipeline::RecordAllComputeDispatches(cmd, context);
	computePass.End();


	// Remove this once we have the renderpass improvement
	std::unordered_set< Renderer * >	renderers;
	context->GetRenderers(renderers);

	// In OSX, we don't need to provide the framebuffer to the renderpass
	// Will remove when we swap to primary command buffer recording
	forwardPass.Begin(cmd, GetCurrentFrameBuffer(), "All Meshes");
	Renderer * meshRenderer = *renderers.begin();
	forwardPass.BindDescriptorSet(LWGCBinding::Frame, perFrameDescriptorSet);
	forwardPass.BindMaterial(meshRenderer->GetMaterial());
	RenderPipeline::RecordAllMeshRenderers(cmd, context);
			// VkCommandBuffer drawMeshBuffer = meshRenderer->GetCommandBuffer(currentFrame);
			// forwardPass.BeginSecondaryCommandBuffer(drawMeshBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
			// meshRenderer->RecordCommands(drawMeshBuffer);
			// forwardPass.ExecuteCommandBuffer(drawMeshBuffer);
	forwardPass.End();

	RenderPipeline::PresentFrame();
	// RenderPipeline::Render(cameras, context);
}