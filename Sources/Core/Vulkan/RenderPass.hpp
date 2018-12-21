#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "VulkanInstance.hpp"

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
			VulkanInstance *						_instance;
			SwapChain *								_swapChain;
	
			std::vector< VkAttachmentDescription >	_attachments;
			std::vector< VkAttachmentReference >	_references;
			std::vector< VkSubpassDependency >		_dependencies;
			VkAttachmentReference					_depthAttachmentRef;
			uint32_t								_attachmentCount;
			VkCommandBuffer							_commandBuffer;
			DescriptorBindings						_currentBindings;
			Material * 			_currentMaterial;

			void	UpdateDescriptorBindings(void);
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
			void	BindBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType);
			void	BindTexture(const std::string & bindingName, const Texture & texture, VkImageLayout imageLayout, VkDescriptorType descriptorType);
			void	BindSampler(const std::string & bindingName, VkSampler sampler);
			void	SetCurrentCommandBuffers(const VkCommandBuffer commandBuffer);
			void	EnqueueCommand(VkCommandBuffer secondaryCommandBuffer);
			void	Cleanup(void) noexcept;
			void	Create(void);
			void	ClearBindings(void);
	
			VkRenderPass	GetRenderPass(void) const noexcept;
	
			static VkAttachmentDescription	GetDefaultColorAttachment(VkFormat format) noexcept;
			static VkAttachmentDescription	GetDefaultDepthAttachment(VkFormat format) noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, RenderPass const & r);
}	

#include "SwapChain.hpp"
