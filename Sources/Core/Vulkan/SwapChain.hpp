#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include GLFW_INCLUDE
#include "VulkanInstance.hpp"
#include "RenderPass.hpp"
#include "VulkanSurface.hpp"

namespace LWGC
{
	class		SwapChain
	{
		friend class RenderPipeline;
	
		private:
			VkSwapchainKHR					_swapChain;
			VkSurfaceKHR					_surface;
			std::vector< VkImage >			_images;
			VkFormat						_imageFormat;
			VkExtent2D						_extent;
			std::vector< VkImageView >		_imageViews;
			std::vector< VkFramebuffer >	_framebuffers;
			VkDevice						_device;
			VulkanInstance *				_instance;
			GLFWwindow *					_window;
			uint32_t						_imageCount;
	
			VkImage							_depthImage;
			VkDeviceMemory					_depthImageMemory;
			VkImageView						_depthImageView;
	
	
			void				CreateSwapChain(void);
			void				CreateImageViews(void) noexcept;
			VkExtent2D			ChooseExtent(void) noexcept;
			VkSurfaceFormatKHR	ChooseSurfaceFormat(void) noexcept;
			VkPresentModeKHR	ChoosePresentMode(void) noexcept;
			// Must be called from the RenderPass class as it depends from it
			void				CreateFrameBuffers(const RenderPass & renderPass);
			void				CreateDepthResources(void);
			void				TransitionDepthImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void				Initialize(void);
	
		public:
			SwapChain(void);
			SwapChain(const SwapChain &) = delete;
			virtual ~SwapChain(void);
	
			SwapChain &	operator=(SwapChain const & src) = delete;
	
			void	Initialize(const VulkanSurface & surface);
			void	Create(void);
			void	Cleanup(void) noexcept;
	
			VkSwapchainKHR						GetSwapChain(void) const noexcept;
			const std::vector< VkImage >		GetImages(void) const noexcept;
			VkFormat							GetImageFormat(void) const noexcept;
			VkExtent2D							GetExtent(void) const noexcept;
			const std::vector< VkImageView >	GetImageViews(void) const noexcept;
			const std::vector< VkFramebuffer >	GetFramebuffers(void) const noexcept;
			uint32_t							GetImageCount(void) const noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, SwapChain const & r);
}
