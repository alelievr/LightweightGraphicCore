#include "ImGUIWrapper.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

ImGUIWrapper::ImGUIWrapper(void)
{
	_instance = nullptr;
	_device = VK_NULL_HANDLE;
	_descriptorPool = VK_NULL_HANDLE;
	_swapChain = nullptr;
	_wd = {};
}

ImGUIWrapper::~ImGUIWrapper(void)
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void		ImGUIWrapper::Initialize(SwapChain * swapChain, VulkanSurface * surface)
{
	_instance = VulkanInstance::Get();
	_device = _instance->GetDevice();
	_surface = surface;
	_swapChain = swapChain;

	CreateDescriptorPool();
	InitImGUI();
	UploadFonts();
}

void		ImGUIWrapper::InitImGUI(void)
{
	VkPhysicalDevice	physicalDevice = _instance->GetPhysicalDevice();
	const auto &		extent = _swapChain->GetExtent();

	ImGui_ImplVulkanH_CreateWindowDataCommandBuffers(physicalDevice, _device, _instance->GetQueueIndex(), &_wd, VK_NULL_HANDLE);
	_wd.Surface = _surface->GetSurface();
	printf("physicalDevice: %p\n", physicalDevice);
	printf("surface: %p\n", _wd.Surface);
    ImGui_ImplVulkanH_CreateWindowDataSwapChainAndFramebuffer(physicalDevice, _device, &_wd, VK_NULL_HANDLE, extent.width, extent.height);

	// // Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// TODO framebuffer resize

	// // Is this important ?
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	_queue = _instance->GetQueue();

	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = _instance->GetInstance();
	initInfo.PhysicalDevice = physicalDevice;
	initInfo.Device = _instance->GetDevice();
	initInfo.QueueFamily = _instance->GetQueueIndex();
	initInfo.Queue = _queue;
	initInfo.PipelineCache = VK_NULL_HANDLE;
	initInfo.DescriptorPool = _descriptorPool;
	initInfo.Allocator = VK_NULL_HANDLE;
	initInfo.CheckVkResultFn = [](VkResult ret) {
		if (ret == VK_SUCCESS)
			return ;
		std::cout << "Error: " << ret << std::endl;
	};

	ImGui_ImplGlfw_InitForVulkan(_surface->GetWindow(), true);
	ImGui_ImplVulkan_Init(&initInfo, _wd.RenderPass);
	ImGui::StyleColorsDark();
}

void		ImGUIWrapper::UploadFonts(void)
{
	VkCommandPool command_pool = _wd.Frames[_wd.FrameIndex].CommandPool;
	VkCommandBuffer command_buffer = _wd.Frames[_wd.FrameIndex].CommandBuffer;

	VkResult err = vkResetCommandPool(_device, command_pool, 0);
	Vk::CheckResult(err, "ImGUI Upload fonts failed");
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	err = vkBeginCommandBuffer(command_buffer, &begin_info);
	Vk::CheckResult(err, "ImGUI Upload fonts failed");

	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

	VkSubmitInfo end_info = {};
	end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	end_info.commandBufferCount = 1;
	end_info.pCommandBuffers = &command_buffer;
	err = vkEndCommandBuffer(command_buffer);
	Vk::CheckResult(err, "ImGUI Upload fonts failed");
	err = vkQueueSubmit(_queue, 1, &end_info, VK_NULL_HANDLE);
	Vk::CheckResult(err, "ImGUI Upload fonts failed");

	err = vkDeviceWaitIdle(_device);
	Vk::CheckResult(err, "ImGUI Upload fonts failed");
	ImGui_ImplVulkan_InvalidateFontUploadObjects();
}

void		ImGUIWrapper::CreateDescriptorPool(void)
{
	VkDescriptorPoolSize poolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
	poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
	poolInfo.pPoolSizes = poolSizes;
	Vk::CheckResult(vkCreateDescriptorPool(_device, &poolInfo, VK_NULL_HANDLE, &_descriptorPool), "Create ImGUI descriptor pool failed");
}

void		ImGUIWrapper::BeginFrame(void)
{
	// Draw GUI on top of everything (after pipeline rendering)
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void		ImGUIWrapper::EndFrame(void)
{
	VkResult err;

	ImGui::Render();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	memcpy(&_wd.ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));

	VkSemaphore& image_acquired_semaphore  = _wd.Frames[_wd.FrameIndex].ImageAcquiredSemaphore;

	// Is this needed as we already wait in the defaut rendering ?
	ImGui_ImplVulkanH_FrameData* fd = &_wd.Frames[_wd.FrameIndex];
    {
		err = vkWaitForFences(_device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);	// wait indefinitely instead of periodically checking
		Vk::CheckResult(err, "Error while rendering GUI");

		err = vkResetFences(_device, 1, &fd->Fence);
		Vk::CheckResult(err, "Error while rendering GUI");
	}
	{
		err = vkResetCommandPool(_device, fd->CommandPool, 0);
		Vk::CheckResult(err, "Error while rendering GUI");
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		Vk::CheckResult(err, "Error while rendering GUI");
	}
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = _wd.RenderPass;
		info.framebuffer = _wd.Framebuffer[_wd.FrameIndex];
		info.renderArea.extent.width = _wd.Width;
		info.renderArea.extent.height = _wd.Height;
		info.clearValueCount = 1;
		info.pClearValues = &_wd.ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record Imgui Draw Data and draw funcs into command buffer
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &fd->RenderCompleteSemaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		Vk::CheckResult(err, "Error while rendering GUI");
		err = vkQueueSubmit(_queue, 1, &info, fd->Fence);
		Vk::CheckResult(err, "Error while rendering GUI");
	}

	// Will need to use the semaphores inside the Vulkan render pipeline :(
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &fd->RenderCompleteSemaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &_wd.Swapchain;
	info.pImageIndices = &_wd.FrameIndex;
	Vk::CheckResult(vkQueuePresentKHR(_queue, &info), "ImGUI present failed");
}


std::ostream &	operator<<(std::ostream & o, ImGUIWrapper const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
