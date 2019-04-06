#include "SwapChain.hpp"
#include "Vk.hpp"

#include <array>
#include <limits>

using namespace LWGC;

SwapChain::SwapChain(void) : _instance(nullptr), _window(nullptr), _imageCount(0)
{
	this->_swapChain = VK_NULL_HANDLE;
	this->_device = VK_NULL_HANDLE;
}

SwapChain::~SwapChain(void)
{
	Cleanup();
}

void		SwapChain::Initialize(const VulkanSurface & surface)
{
	_instance = VulkanInstance::Get();
	_surface = surface.GetSurface();
	_device = _instance->GetDevice();
	_window = surface.GetWindow();
	Create();
}

void		SwapChain::Create(void)
{
	CreateSwapChain();
	CreateImageViews();
}

void		SwapChain::Cleanup(void) noexcept
{
	vkDestroyImageView(_device, _depthImageView, nullptr);
	vkDestroyImage(_device, _depthImage, nullptr);
	vkFreeMemory(_device, _depthImageMemory, nullptr);

	for (size_t i = 0; i < _framebuffers.size(); i++)
		vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);

	for (size_t i = 0; i < _imageViews.size(); i++)
		vkDestroyImageView(_device, _imageViews[i], nullptr);

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}

void		SwapChain::CreateSwapChain(void)
{
	VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat();
	VkPresentModeKHR presentMode = ChoosePresentMode();
	VkExtent2D extent = ChooseExtent();

	const auto & capabilities = _instance->GetSurfaceCapabilities();
	_imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && _imageCount > capabilities.maxImageCount)
	{
	    _imageCount = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;

	createInfo.minImageCount = _imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
	    throw std::runtime_error("failed to create swap chain!");

	vkGetSwapchainImagesKHR(_device, _swapChain, &_imageCount, nullptr);
	_images.resize(_imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &_imageCount, _images.data());

	_imageFormat = surfaceFormat.format;
	_extent = extent;
}

void				SwapChain::CreateImageViews(void) noexcept
{
	_imageViews.resize(_images.size());

	std::cout << "image count: " << _images.size() << std::endl;

	for (uint32_t i = 0; i < _images.size(); i++)
	    _imageViews[i] = Vk::CreateImageView(_images[i], _imageFormat, 1, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
}

// TODO: this may not be in this class
void				SwapChain::TransitionDepthImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = _instance->GetCommandBufferPool()->BeginSingle();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
	    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	    if (Vk::HasStencilComponent(format)) {
	        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	    }
	} else {
	    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
	    barrier.srcAccessMask = 0;
	    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
	    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
	    barrier.srcAccessMask = 0;
	    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	} else {
	    throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		    commandBuffer,
		    sourceStage, destinationStage,
		    0,
		    0, nullptr,
		    0, nullptr,
		    1, &barrier
		    );

	_instance->GetCommandBufferPool()->EndSingle(commandBuffer);
}

void				SwapChain::CreateDepthResources(void)
{
	VkFormat depthFormat = _instance->FindDepthFormat();

	Vk::CreateImage(_extent.width, _extent.height, 1, 1, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory);
	_depthImageView = Vk::CreateImageView(_depthImage, depthFormat, 1, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
	TransitionDepthImageLayout(_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void				SwapChain::CreateFrameBuffers(const RenderPass & renderPass)
{
	_framebuffers.resize(_imageViews.size());

	CreateDepthResources();

	std::cout << "Swapchain size: " << _imageViews.size() << std::endl;

	for (size_t i = 0; i < _imageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {{
		    _imageViews[i],
		    _depthImageView
		}};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass.GetRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _extent.width;
		framebufferInfo.height = _extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_framebuffers[i]) != VK_SUCCESS)
		    throw std::runtime_error("failed to create framebuffer!");
	}
}

VkSurfaceFormatKHR	SwapChain::ChooseSurfaceFormat(void) noexcept
{
	const auto & availableFormats = _instance->GetSupportedSurfaceFormats();

	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

	for (const auto& availableFormat : availableFormats)
	{
	    if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
	    	return availableFormat;
	}

	return availableFormats[0];
}

VkPresentModeKHR	SwapChain::ChoosePresentMode(void) noexcept
{
	const auto & availablePresentModes = _instance->GetSupportedPresentModes();
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto & availablePresentMode : availablePresentModes)
	{
	    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
	        return availablePresentMode;
	    else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
	        bestMode = availablePresentMode;
	}

	return bestMode;
}

VkExtent2D			SwapChain::ChooseExtent(void) noexcept
{
	const auto & capabilities = _instance->GetSurfaceCapabilities();

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	    return capabilities.currentExtent;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);

	VkExtent2D actualExtent = {
	    static_cast<uint32_t>(width),
	    static_cast<uint32_t>(height)
	};

	std::cout << "FrameBuffer size: " << width << " / " << height << std::endl;

	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

	return actualExtent;
}

VkSwapchainKHR					SwapChain::GetSwapChain(void) const noexcept { return (this->_swapChain); }
const std::vector< VkImage >	SwapChain::GetImages(void) const noexcept { return (this->_images); }
VkFormat						SwapChain::GetImageFormat(void) const noexcept { return (this->_imageFormat); }
VkExtent2D						SwapChain::GetExtent(void) const noexcept { return (this->_extent); }
const std::vector< VkImageView >SwapChain::GetImageViews(void) const noexcept { return (this->_imageViews); }
const std::vector< VkFramebuffer >SwapChain::GetFramebuffers(void) const noexcept { return (this->_framebuffers); }
uint32_t						SwapChain::GetImageCount(void) const noexcept { return (this->_imageCount); }

std::ostream &	operator<<(std::ostream & o, SwapChain const & r)
{
	o << "SwapChain of " << r.GetImageCount() << " framebuffers" << std::endl;
	(void)r;
	return (o);
}
