#include "ImGUIWrapper.hpp"

#include "Core/Vulkan/Vk.hpp"
#include "Core/EventSystem.hpp"
#include "Core/Rendering/RenderPipeline.hpp"
#include "Core/Rendering/RenderPipelineManager.hpp"

#define IMGUI_MAX_ELEMENTS	1000

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
	_renderPass.Initialize();

	InitImGUIFrameDatas();
	_wd.Surface = _surface->GetSurface();

	// Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Is this important ?
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

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

	auto pipeline = RenderPipelineManager::currentRenderPipeline;
	ImGui_ImplGlfw_InitForVulkan(_surface->GetWindow(), false);
	auto es = EventSystem::Get();
	es->onMouseClick.AddListener([](glm::vec2 pos, int button, ButtonAction action){
		(void)pos;
		ImGui_ImplGlfw_MouseButtonCallback(nullptr, button, (int)action, 0);
	});
	es->onScroll.AddListener([](double xOffset, double yOffset){
		ImGui_ImplGlfw_ScrollCallback(nullptr, xOffset, yOffset);
	});
	es->onKey.AddListener([](KeyCode key, ButtonAction action, int mods){
		ImGui_ImplGlfw_KeyCallback(nullptr, (int)key, 0, (int)action, mods);
	});
	es->onChar.AddListener([](uint32_t c){
		ImGui_ImplGlfw_CharCallback(nullptr, c);
	});
	ImGui_ImplVulkan_Init(&initInfo, pipeline->GetRenderPass()->GetRenderPass());
	ImGui::StyleColorsDark();
}

void		ImGUIWrapper::InitImGUIFrameDatas(void)
{
	_wd.Swapchain = _swapChain->GetSwapChain();
	_wd.BackBufferCount = _swapChain->GetImageCount();

	_wd.Width = _swapChain->GetExtent().width;
	_wd.Height = _swapChain->GetExtent().height;

	for (uint32_t i = 0; i < _wd.BackBufferCount; i++)
		_wd.BackBuffer[i] = _swapChain->GetImages()[i];

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = _swapChain->GetImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // We don't want the GUI to clear the frame before
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	_renderPass.AddAttachment(
		colorAttachment,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	);
	_renderPass.SetDepthAttachment(
		RenderPass::GetDefaultDepthAttachment(_instance->FindDepthFormat()),
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	);

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	_renderPass.AddDependency(dependency);
	_renderPass.Create();
	_wd.RenderPass = _renderPass.GetRenderPass();

	for (uint32_t i = 0; i < _swapChain->GetImageCount(); i++)
	{
		auto frame = _wd.Frames + i;

		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolInfo.queueFamilyIndex = _instance->GetQueueIndex();
		Vk::CheckResult(vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &frame->CommandPool), "Can't create ImGUI command pool");

		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandPool = frame->CommandPool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = 1;
		Vk::CheckResult(vkAllocateCommandBuffers(_device, &commandBufferInfo, &frame->CommandBuffer), "Can't create ImGUI command buffer");

		_wd.BackBufferView[i] = _swapChain->GetImageViews()[i];
		_wd.Framebuffer[i] = _swapChain->GetFramebuffers()[i];
	}
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
		{ VK_DESCRIPTOR_TYPE_SAMPLER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, IMGUI_MAX_ELEMENTS },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, IMGUI_MAX_ELEMENTS }
	};
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = IMGUI_MAX_ELEMENTS * IM_ARRAYSIZE(poolSizes);
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

	ImGui_ImplVulkanH_FrameData* fd = &_wd.Frames[_wd.FrameIndex];
	{
		err = vkResetCommandBuffer(fd->CommandBuffer, 0);
		err = vkResetCommandPool(_device, fd->CommandPool, 0);
		Vk::CheckResult(err, "Error while rendering GUI");
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
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
		info.clearValueCount = 2;
		info.pClearValues = &_wd.ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record Imgui Draw Data and draw funcs into command buffer
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		err = vkEndCommandBuffer(fd->CommandBuffer);
		Vk::CheckResult(err, "Error while rendering GUI");
		RenderPipelineManager::currentRenderPipeline->EnqueueFrameCommandBuffer(fd->CommandBuffer);
	}

	_wd.FrameIndex = (_wd.FrameIndex + 1) % _swapChain->GetImageCount();
}


std::ostream &	operator<<(std::ostream & o, ImGUIWrapper const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
