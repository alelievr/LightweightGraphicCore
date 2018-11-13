#pragma once

#include <iostream>
#include <string>

#include "VulkanInstance.hpp"

namespace LWGC
{
	class SwapChain;
	
	class		RenderPass
	{
		private:
			VkRenderPass							_renderPass;
			VulkanInstance *						_instance;
			SwapChain *								_swapChain;
	
			std::vector< VkAttachmentDescription >	_attachments;
			std::vector< VkAttachmentReference >	_references;
			std::vector< VkSubpassDependency >		_dependencies;
			VkAttachmentReference					_depthAttachmentRef;
			uint32_t								_attachmentCount;
	
		public:
			RenderPass(void);
			RenderPass(const RenderPass &) = delete;
			virtual ~RenderPass(void);
	
			RenderPass &	operator=(RenderPass const & src) = delete;
	
			void	Initialize(SwapChain * swapChain) noexcept;
			void	AddAttachment(const VkAttachmentDescription & attachment, VkImageLayout finalLayout) noexcept;
			void	SetDepthAttachment(const VkAttachmentDescription & attachment, VkImageLayout layout) noexcept;
			void	AddDependency(const VkSubpassDependency & dependency) noexcept;
			void	Cleanup(void) noexcept;
			void	Create(void);
	
			VkRenderPass	GetRenderPass(void) const noexcept;
	
			static VkAttachmentDescription	GetDefaultColorAttachment(VkFormat format) noexcept;
			static VkAttachmentDescription	GetDefaultDepthAttachment(VkFormat format) noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, RenderPass const & r);
}	

#include "SwapChain.hpp"
