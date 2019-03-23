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

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = fractalTexture->GetImage();

	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;

	heavyComputeShader.AddImageBarrier(barrier, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	asyncComputeSet.AddBinding(0, fractalTexture, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

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
		auto asyncCmd = asyncComputePool.BeginSingle();

		heavyComputeShader.Dispatch(asyncCmd, 4096, 4096, 1);

		vkResetFences(device, 1, &heavyComputeFence);

		asyncComputePool.EndSingle(asyncCmd); // fence

		// TODO: test this !
		// vkWaitForFences(device, 1, &heavyComputeFence, VK_TRUE, -1);
		// vkQueueWaitIdle(asyncComputeQueue);
		// vkFreeCommandBuffers(device, asyncComputePool._commandPool, 1, &asyncCmd);
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
		forwardPass.BindDescriptorSet("asyncTexture", asyncComputeSet);
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