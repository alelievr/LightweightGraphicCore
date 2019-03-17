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

			bool	BindDescriptorSet(const uint32_t binding, VkDescriptorSet set);

		public:
			RenderPass(void);
			RenderPass(const RenderPass &) = delete;
			virtual ~RenderPass(void);

			RenderPass &	operator=(RenderPass const & src) = delete;

			// Lifecycle
			void	Initialize(SwapChain * swapChain) noexcept;
			void	Create(bool computeOnly = false);
			void	Cleanup(void) noexcept;
			
			// attachments
			void	AddAttachment(const VkAttachmentDescription & attachment, VkImageLayout finalLayout) noexcept;
			void	SetDepthAttachment(const VkAttachmentDescription & attachment, VkImageLayout layout) noexcept;
			void	AddDependency(const VkSubpassDependency & dependency) noexcept;

			// Bindings
			bool	BindDescriptorSet(const std::string & name, VkDescriptorSet set);
			void	BindMaterial(Material * material);
			void	ClearBindings(void);
			void	SetClearColor(const Color & color, float depth, uint32_t stencil);
			void	UpdateDescriptorBindings(void);
			
			// API
			void	Begin(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, const std::string & passName);
			void	End(void);

			VkRenderPass	GetRenderPass(void) const noexcept;
			VkCommandBuffer	GetCommandBuffer(void) const noexcept;

			static VkAttachmentDescription	GetDefaultColorAttachment(VkFormat format) noexcept;
			static VkAttachmentDescription	GetDefaultDepthAttachment(VkFormat format) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, RenderPass const & r);
}

#include "SwapChain.hpp"
