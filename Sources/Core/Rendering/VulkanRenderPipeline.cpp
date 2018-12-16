#include "VulkanRenderPipeline.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Components/ComputeDispatcher.hpp"

#include "Core/PrimitiveMeshFactory.hpp"

#include <cmath>
#include <unordered_set>

using namespace LWGC;

VulkanRenderPipeline *					VulkanRenderPipeline::pipelineInstance = nullptr;

VulkanRenderPipeline * VulkanRenderPipeline::Get() { return pipelineInstance; }

VulkanRenderPipeline::VulkanRenderPipeline(void) : framebufferResized(false)
{
	swapChain = VK_NULL_HANDLE;
	instance = VK_NULL_HANDLE;
	pipelineInstance = this;
}

VulkanRenderPipeline::~VulkanRenderPipeline(void)
{
	vkDeviceWaitIdle(device);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyBuffer(device, uniformPerFrame.buffer, nullptr);
	vkFreeMemory(device, uniformPerFrame.memory, nullptr);
}

void                VulkanRenderPipeline::Initialize(SwapChain * swapChain)
{
    this->instance = VulkanInstance::Get();
	this->device = instance->GetDevice();
    this->swapChain = swapChain;
	renderPass.Initialize(swapChain);
	CreateRenderPass();

	// Allocate primary command buffers
	instance->GetCommandBufferPool()->Allocate(VK_COMMAND_BUFFER_LEVEL_PRIMARY, swapChainCommandBuffers, swapChain->GetImageCount());

	// Allocate LWGC_PerFrame uniform buffer
	Vk::CreateBuffer(sizeof(LWGC_PerFrame), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformPerFrame.buffer, uniformPerFrame.memory);

	CreateDescriptorSets();
	CreatePerFrameDescriptorSet();
}

void				VulkanRenderPipeline::CreateDescriptorSets(void) {}

void				VulkanRenderPipeline::CreatePerFrameDescriptorSet(void) noexcept
{
	auto layoutBinding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL);
	Vk::CreateDescriptorSetLayout({layoutBinding}, perFrameDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = instance->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = &perFrameDescriptorSetLayout; // First layout set is per frame cbuffer

	if (vkAllocateDescriptorSets(device, &allocInfo, &perFrameDescriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniformPerFrame.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerFrame);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = perFrameDescriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
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

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
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
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	renderPass.SetCurrentCommandBuffers(commandBuffer);

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void			VulkanRenderPipeline::EndRenderPass(void)
{
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

static bool my_tool_active;
static float my_color[4];

void			VulkanRenderPipeline::RenderGUI(void) noexcept
{
	// tests:
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O"))
			{ /* Do stuff */
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{ /* Do stuff */
			}
			if (ImGui::MenuItem("Close", "Ctrl+W"))
			{
				my_tool_active = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", my_color);

	// Plot some values
	const float my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}

void			VulkanRenderPipeline::CreateSyncObjects(void)
{
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
	std::unordered_set< Renderer * >	renderers;
	vkDeviceWaitIdle(device);

	swapChain->Cleanup();
	renderPass.Cleanup();

	// Rebuild all Material graphic pipelines
	// TODO: do not work with compute dispatchers
	renderContext.GetRenderers(renderers);
	
	for (auto & meshRenderer : renderers)
		meshRenderer->CleanupPipeline();

	instance->UpdateSurface();
	swapChain->Create();
	CreateRenderPass();

	for (auto & meshRenderer : renderers)
		meshRenderer->CreatePipeline();
}

void			VulkanRenderPipeline::UpdatePerframeUnformBuffer(void) noexcept
{
	perFrame.time.x = static_cast< float >(glfwGetTime());
	perFrame.time.y = sin(perFrame.time.x);
	perFrame.time.z = cos(perFrame.time.x);
	perFrame.time.w = 0; // TODO: delta time

	// Upload datas to GPU
	void* data;
	vkMapMemory(device, uniformPerFrame.memory, 0, sizeof(LWGC_PerFrame), 0, &data);
	memcpy(data, &perFrame, sizeof(LWGC_PerFrame));
	vkUnmapMemory(device, uniformPerFrame.memory);
}

void			VulkanRenderPipeline::RenderInternal(const std::vector< Camera * > & cameras, RenderContext & context)
{
	UpdatePerframeUnformBuffer();

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

	commandBuffer = swapChainCommandBuffers[imageIndex];

	Render(cameras, context);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

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

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(instance->GetQueue(), &presentInfo);

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
	if (cameras.empty())
		throw std::runtime_error("No camera for rendering !");

	BeginRenderPass();
	
	renderPass.BindDescriptorSet(LWGCBinding::Frame, perFrameDescriptorSet);

	// Run compute pass as it does not depends on any cameras
	std::unordered_set< ComputeDispatcher * >	computeDispatchers;

	context.GetComputeDispatchers(computeDispatchers);

	for (auto & compute : computeDispatchers)
	{
		auto m = compute->GetMaterial();

		renderPass.BindMaterial(m);

		renderPass.EnqueueCommand(compute->GetCommandBuffer());
	}

	for (const auto camera : cameras)
	{
		std::unordered_set< Renderer * >	renderers;
		
		renderPass.BindDescriptorSet(LWGCBinding::Camera, camera->GetDescriptorSet());

		context.GetRenderers(renderers);

		for (const auto & meshRenderer : renderers)
		{
			auto m = meshRenderer->GetMaterial();
			renderPass.BindDescriptorSet(LWGCBinding::Object, meshRenderer->GetDescriptorSet());
			renderPass.BindMaterial(m);

			// TODO: put an event listener in MeshRenderer and update uniforms from there
			m->UpdateUniformBuffer();
			
			renderPass.EnqueueCommand(meshRenderer->GetDrawCommandBuffer());
		}
	}

	EndRenderPass();

	renderPass.ClearBindings();
}

SwapChain *		VulkanRenderPipeline::GetSwapChain(void) { return swapChain; }
RenderPass *	VulkanRenderPipeline::GetRenderPass(void) { return &renderPass; }
