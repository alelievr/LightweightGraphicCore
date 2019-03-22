#include "ForwardRenderPipeline.hpp"
#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

Texture2D * fractalTexture;

void	ForwardRenderPipeline::Initialize(SwapChain * swapChain)
{
	RenderPipeline::Initialize(swapChain);

	heavyComputeShader.LoadShader("Shaders/Compute/Heavy.hlsl");
	heavyComputeFence = Vk::CreateFence(true);

	// Allocate an async command queue (the device must have more than one queue to run the application)
	instance->AllocateDeviceQueue(asyncComputeQueue, asyncComputeQueueIndex);
	asyncComputePool.Initialize(asyncComputeQueue, asyncComputeQueueIndex);
	asyncCommandBuffer = asyncComputePool.Allocate(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	fractalTexture = Texture2D::Create(2048, 2048, VK_FORMAT_R8G8B8A8_SNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	heavyComputeShader.SetTexture("fractal", fractalTexture, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);

	// Setup the render passes we uses:
	SetupRenderPasses();
}

void	ForwardRenderPipeline::SetupRenderPasses()
{
	// Compute pass (for each frame)
	computePass.Initialize(swapChain);
	// For compute we don't need to call Create on renderpass because we don't need one to run a compute shader

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

	VkResult heavyComputeResult = vkGetFenceStatus(device, heavyComputeFence);
	if (heavyComputeResult == VK_SUCCESS)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(asyncCommandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		heavyComputeShader.Dispatch(asyncCommandBuffer, 512, 512, 1);

		if (vkEndCommandBuffer(asyncCommandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &asyncCommandBuffer;

		vkResetFences(device, 1, &heavyComputeFence);

		Vk::CheckResult(vkQueueSubmit(asyncComputeQueue, 1, &submitInfo, heavyComputeFence), "Failed to submit queue");

	} else if (heavyComputeResult == VK_NOT_READY) {
		// std::cout << "Not ready !\n";
	}
	else
	{
		std::cerr << "Device lost error !\n";
	}

	// Process the compute shader before everything:
	computePass.Begin(cmd, VK_NULL_HANDLE, "All Computes");
	{
		computePass.BindDescriptorSet(LWGCBinding::Frame, perFrameDescriptorSet);
		RenderPipeline::RecordAllComputeDispatches(computePass, context);
	}
	computePass.End();

	forwardPass.Begin(cmd, GetCurrentFrameBuffer(), "All Cameras");
	{
		forwardPass.BindDescriptorSet(LWGCBinding::Frame, perFrameDescriptorSet);
		for (const auto camera : cameras)
		{

			beginCameraRendering.Invoke(camera);
			forwardPass.BindDescriptorSet(LWGCBinding::Camera, camera->GetDescriptorSet());

			RenderPipeline::RecordAllMeshRenderers(forwardPass, context);

			endCameraRendering.Invoke(camera);
		}
	}
	forwardPass.End();
}