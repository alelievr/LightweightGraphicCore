#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "VulkanInstance.hpp"
#include "Utils/Color.hpp"

namespace LWGC
{
	class SwapChain;
	class Material;
	class Texture;

	class		RenderPass
	{
		friend class Material; // For binding

		private:
			struct DescriptorSet
			{
				VkDescriptorSet	set;
				bool			hasChanged;
			};

			using DescriptorBindings = std::unordered_map< std::string, DescriptorSet >;

			VkRenderPass							_renderPass;
			VkFramebuffer							_framebuffer;
			VulkanInstance *						_instance;

			std::vector< VkAttachmentDescription >	_attachments;
			std::vector< VkAttachmentReference >	_references;
			std::vector< VkSubpassDependency >		_dependencies;
			VkAttachmentReference					_depthAttachmentRef;
			uint32_t								_attachmentCount;
			VkCommandBuffer							_commandBuffer;
			DescriptorBindings						_currentBindings;
			Material * 								_currentMaterial;
			std::vector< VkClearValue >				_clearValues;
			SwapChain *								_swapChain;

			void	UpdateDescriptorBindings(VkCommandBuffer cmd);
			bool	BindDescriptorSet(const uint32_t binding, VkDescriptorSet set);

		public:
			RenderPass(void);
			RenderPass(const RenderPass &) = delete;
			virtual ~RenderPass(void);

			RenderPass &	operator=(RenderPass const & src) = delete;

			void	Initialize(SwapChain * swapChain) noexcept;
			void	AddAttachment(const VkAttachmentDescription & attachment, VkImageLayout finalLayout) noexcept;
			void	SetDepthAttachment(const VkAttachmentDescription & attachment, VkImageLayout layout) noexcept;
			void	AddDependency(const VkSubpassDependency & dependency) noexcept;
			bool	BindDescriptorSet(const std::string & name, VkDescriptorSet set);
			void	BindMaterial(Material * material);
			void	Begin(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, const std::string & passName);
			void	End(void);
			void	BeginSecondaryCommandBuffer(VkCommandBuffer cmd, VkCommandBufferUsageFlagBits commandBufferUsage);
			void	ExecuteCommandBuffer(VkCommandBuffer cmd);
			void	Cleanup(void) noexcept;
			void	Create(void);
			void	ClearBindings(void);
			void	SetClearColor(const Color & color, float depth, uint32_t stencil);

			VkRenderPass	GetRenderPass(void) const noexcept;

			static VkAttachmentDescription	GetDefaultColorAttachment(VkFormat format) noexcept;
			static VkAttachmentDescription	GetDefaultDepthAttachment(VkFormat format) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, RenderPass const & r);
}

#include "SwapChain.hpp"
