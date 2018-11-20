#include "VulkanRenderPipeline.hpp"
#include "Core/Components/MeshRenderer.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

#include <unordered_set>

using namespace LWGC;

VulkanRenderPipeline * VulkanRenderPipeline::pipelineInstance = nullptr;

VulkanRenderPipeline * VulkanRenderPipeline::Get() { return pipelineInstance; }

VulkanRenderPipeline::VulkanRenderPipeline(void) : framebufferResized(false)
{
	swapChain = VK_NULL_HANDLE;
	instance = VK_NULL_HANDLE;
	pipelineInstance = this;
}

VulkanRenderPipeline::~VulkanRenderPipeline(void)
{
	vkDeviceWaitIdle(instance->GetDevice());

	auto device = instance->GetDevice();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
}

void                VulkanRenderPipeline::Initialize(SwapChain * swapChain)
{
    this->instance = VulkanInstance::Get();
    this->swapChain = swapChain;
	renderPass.Initialize(swapChain);
	CreateRenderPass();

	// Allocate primary command buffers
	printf("Allocated command buffers !\n");
	instance->GetGraphicCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_PRIMARY, swapChainCommandBuffers, swapChain->GetImageCount());
}

void				VulkanRenderPipeline::CreateRenderPass(void)
{
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

void			VulkanRenderPipeline::BeginRenderPass(void)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	if (vkBeginCommandBuffer(graphicCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass.GetRenderPass();
	renderPassInfo.framebuffer = swapChain->GetFramebuffers()[currentFrame]; // TODO: simplify this
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChain->GetExtent();

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(graphicCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void			VulkanRenderPipeline::EndRenderPass(void)
{
	vkCmdEndRenderPass(graphicCommandBuffer);

	if (vkEndCommandBuffer(graphicCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void			VulkanRenderPipeline::CreateSyncObjects(void)
{
	auto device = instance->GetDevice();
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}

	printf("Semaphores created !\n");
}

void			VulkanRenderPipeline::RecreateSwapChain(RenderContext & renderContext)
{
	std::unordered_set< MeshRenderer * >	meshRenderers;
	auto device = instance->GetDevice();
	vkDeviceWaitIdle(device);

	swapChain->Cleanup();
	renderPass.Cleanup();

	// Rebuild all Material graphic pipelines
	renderContext.GetMeshRenderers(meshRenderers);
	
	for (auto & meshRenderer : meshRenderers)
		meshRenderer->CleanupGraphicPipeline();

	instance->UpdateSurface();
	swapChain->Create();
	CreateRenderPass();

	for (auto & meshRenderer : meshRenderers)
		meshRenderer->CleanupGraphicPipeline();
}

void			VulkanRenderPipeline::RenderInternal(const std::vector< Camera * > & cameras, RenderContext & context)
{
	auto device = instance->GetDevice();
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	// TODO: maybe put this function inside the swapChain class ?
	VkResult result = vkAcquireNextImageKHR(device, swapChain->GetSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain(context);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	graphicCommandBuffer = swapChainCommandBuffers[imageIndex];

	Render(cameras, context);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &graphicCommandBuffer;

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(instance->GetGraphicQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit graphic queue!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain->GetSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(instance->GetPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		framebufferResized = false;
		RecreateSwapChain(context);
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void	VulkanRenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext & context)
{
	std::vector< VkCommandBuffer > drawBuffers;
	std::unordered_set< MeshRenderer * >	meshRenderers;

	BeginRenderPass();

	context.GetMeshRenderers(meshRenderers);

	for (const auto & meshRenderer : meshRenderers)
	{
		meshRenderer->GetMaterial()->UpdateUniformBuffer();
		
		drawBuffers.push_back(meshRenderer->GetDrawCommandBuffer());
	}

	vkCmdExecuteCommands(graphicCommandBuffer, drawBuffers.size(), drawBuffers.data());

	EndRenderPass();
}

SwapChain *		VulkanRenderPipeline::GetSwapChain(void) { return swapChain; }
RenderPass *	VulkanRenderPipeline::GetRenderPass(void) { return &renderPass; }