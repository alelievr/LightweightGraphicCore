#include "RenderPipeline.hpp"

#include "Core/Components/MeshRenderer.hpp"
#include "Core/Components/ComputeDispatcher.hpp"
#include "Core/Components/ImGUIPanel.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Handles/Selection.hpp"
#include "Core/Handles/Tools.hpp"
#include "Core/Handles/HandleManager.hpp"
#include "Core/MaterialTable.hpp"
#include "Core/Time.hpp"

#include <cmath>
#include <unordered_set>

using namespace LWGC;

RenderPipeline::RenderPipeline(void) : _initialized(false), framebufferResized(false)
{
	swapChain = VK_NULL_HANDLE;
	instance = VK_NULL_HANDLE;
	currentCamera = nullptr;
}

RenderPipeline::~RenderPipeline(void)
{
	vkDeviceWaitIdle(device);

	for (size_t i = 0; i < swapChain->GetImageCount(); i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyBuffer(device, _uniformPerFrame.buffer, nullptr);
	vkFreeMemory(device, _uniformPerFrame.memory, nullptr);
}

void                RenderPipeline::Initialize(SwapChain * swapChain)
{
    this->instance = VulkanInstance::Get();
	this->device = instance->GetDevice();
    this->swapChain = swapChain;
	renderPass.Initialize();
	CreateRenderPass();

	// Allocate primary command buffers
	instance->GetCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_PRIMARY, _swapChainCommandBuffers, swapChain->GetImageCount());

	// Allocate LWGC_PerFrame uniform buffer
	Vk::CreateBuffer(sizeof(LWGC_PerFrame), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _uniformPerFrame.buffer, _uniformPerFrame.memory);

	CreatePerFrameDescriptorSet();

	// InitializeHandles();

	_initialized = true;
}

void				RenderPipeline::InitializeHandles(void) noexcept
{
	Selection::Initialize();
	Tools::Initialize();
	HandleManager::Initialize();
}

void				RenderPipeline::CreatePerFrameDescriptorSet(void)
{
	auto layoutBinding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL);
	Vk::CreateDescriptorSetLayout({layoutBinding}, _perFrameDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = instance->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = &_perFrameDescriptorSetLayout; // First layout set is per frame cbuffer

	if (vkAllocateDescriptorSets(device, &allocInfo, &_perFrameDescriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformPerFrame.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerFrame);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _perFrameDescriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

void				RenderPipeline::CreateRenderPass(void)
{
	renderPass.AddAttachment(
		RenderPass::GetDefaultColorAttachment(swapChain->GetImageFormat()),
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	);
	renderPass.SetDepthAttachment(
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

	renderPass.AddDependency(dependency);

	renderPass.Create();
	swapChain->CreateFrameBuffers(renderPass);
}

void			RenderPipeline::BeginRenderPass(RenderContext * context)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	if (vkBeginCommandBuffer(frameCommandBuffers[0], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	framebuffer = swapChain->GetFramebuffers()[currentFrame]; // TODO: simplify this

	renderPass.Begin(frameCommandBuffers[0], framebuffer, "Main Pass");

	// Run all compute shaders before begin render pass:
	std::unordered_set< ComputeDispatcher * >	computeDispatchers;
	context->GetComputeDispatchers(computeDispatchers);

	// Bind frame infos for compute shaders
	renderPass.BindDescriptorSet(LWGCBinding::Frame, _perFrameDescriptorSet);

	for (auto & compute : computeDispatchers)
	{
		auto m = compute->GetMaterial();

		renderPass.BindMaterial(m);

		auto cmd = compute->GetCommandBuffer();
		renderPass.BeginSecondaryCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		compute->RecordCommands(cmd);
		renderPass.ExecuteCommandBuffer(cmd);
	}

	renderPass.ClearBindings();

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass.GetRenderPass();
	renderPassInfo.framebuffer = framebuffer;
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChain->GetExtent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(frameCommandBuffers[0], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void			RenderPipeline::EndRenderPass(void)
{
	renderPass.End();

	vkCmdEndRenderPass(frameCommandBuffers[0]);

	if (vkEndCommandBuffer(frameCommandBuffers[0]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void			RenderPipeline::RenderGUI(RenderContext * context) noexcept
{
	std::unordered_set< ImGUIPanel * >	imGUIPanels;

	context->GetComponentSet< ImGUIPanel * >(imGUIPanels);

	for (auto imGUIPanel : imGUIPanels)
	{
		imGUIPanel->DrawImGUI();
	}
}

void			RenderPipeline::CreateSyncObjects(void)
{
	size_t frameBufferCount = swapChain->GetImageCount();
	imageAvailableSemaphores.resize(frameBufferCount);
	renderFinishedSemaphores.resize(frameBufferCount);
	inFlightFences.resize(frameBufferCount);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < frameBufferCount; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}

	printf("Semaphores created !\n");
}

void			RenderPipeline::RecreateSwapChain(void)
{
	vkDeviceWaitIdle(device);

	swapChain->Cleanup();
	renderPass.Cleanup();

	instance->UpdateSurface();
	swapChain->Create();
	CreateRenderPass();

	// Recreate all materials with the new RenderPass
	MaterialTable::Get()->RecreateAll();
}

void			RenderPipeline::UpdatePerframeUnformBuffer(void) noexcept
{
	_perFrame.time.x = static_cast< float >(glfwGetTime());
	_perFrame.time.y = sin(_perFrame.time.x);
	_perFrame.time.z = cos(_perFrame.time.x);
	_perFrame.time.w = Time::GetDeltaTime();

	// Upload datas to GPU
	Vk::UploadToMemory(_uniformPerFrame.memory, &_perFrame, sizeof(LWGC_PerFrame));
}

void			RenderPipeline::RenderInternal(const std::vector< Camera * > & cameras, RenderContext * context)
{
	UpdatePerframeUnformBuffer();

	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	// TODO: maybe put this function inside the swapChain class ?
	VkResult result = vkAcquireNextImageKHR(device, swapChain->GetSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &_imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	frameCommandBuffers.clear();
	frameCommandBuffers.push_back(_swapChainCommandBuffers[_imageIndex]);

	currentCamera = (cameras.size() == 0) ? nullptr : cameras[0];

	Render(cameras, context);
}

void	RenderPipeline::PresentFrame(void)
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = frameCommandBuffers.size();
	submitInfo.pCommandBuffers = frameCommandBuffers.data();

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	Vk::CheckResult(vkResetFences(device, 1, &inFlightFences[currentFrame]), "Reset fence failed");

	Vk::CheckResult(vkQueueSubmit(instance->GetQueue(), 1, &submitInfo, inFlightFences[currentFrame]), "Failed to submit queue");

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain->GetSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &_imageIndex;

	VkResult result = vkQueuePresentKHR(instance->GetQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % swapChain->GetImageCount();
}

void	RenderPipeline::Render(const std::vector< Camera * > & cameras, RenderContext * context)
{
	if (cameras.empty())
		throw std::runtime_error("No camera for rendering !");

	BeginRenderPass(context);

	renderPass.BindDescriptorSet(LWGCBinding::Frame, _perFrameDescriptorSet);

	beginFrameRendering.Invoke();

	for (const auto camera : cameras)
	{
		beginCameraRendering.Invoke(camera);
		std::unordered_set< Renderer * >	renderers;

		renderPass.BindDescriptorSet(LWGCBinding::Camera, camera->GetDescriptorSet());

		context->GetRenderers(renderers);

		for (const auto & meshRenderer : renderers)
		{
			auto m = meshRenderer->GetMaterial();
			renderPass.BindDescriptorSet(LWGCBinding::Object, meshRenderer->GetDescriptorSet());
			renderPass.BindMaterial(m);

			// Get the command buffer, should be empty at this state
			VkCommandBuffer drawMeshBuffer = meshRenderer->GetCommandBuffer(currentFrame);
			renderPass.BeginSecondaryCommandBuffer(drawMeshBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
			meshRenderer->RecordCommands(drawMeshBuffer);
			renderPass.ExecuteCommandBuffer(drawMeshBuffer);
		}

		endCameraRendering.Invoke(camera);
	}

	endFrameRendering.Invoke();

	EndRenderPass();

	renderPass.ClearBindings();
}

void			RenderPipeline::EnqueueFrameCommandBuffer(VkCommandBuffer cmd)
{
	frameCommandBuffers.push_back(cmd);
}

// This must return the render pass compatible with the swapchain (for the final blit)
RenderPass *	RenderPipeline::GetRenderPass(void) { return &renderPass; }
Camera *		RenderPipeline::GetCurrentCamera(void) { return currentCamera; }
bool			RenderPipeline::IsInitialized(void) { return _initialized; }
