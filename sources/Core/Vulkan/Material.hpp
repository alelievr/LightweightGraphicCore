#pragma once

#include <iostream>
#include <string>

#include "Core/Texture.hpp"
#include "Core/Texture2D.hpp"
#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include "VulkanInstance.hpp"
#include "SwapChain.hpp"
#include "Vk.hpp"

struct		UniformBuffers
{
	VkBuffer		buffer;
	VkDeviceMemory	memory;
};

namespace LWGC
{
	class		Material
	{
		private:
			static VkDescriptorPool			_descriptorPool;
			VkDescriptorSet					_descriptorSet;
			VkDescriptorSetLayout			_descriptorSetLayout;
			VkPipelineLayout				_graphicPipelineLayout;
			VkPipeline						_graphicPipeline;
			std::vector< UniformBuffers >	_uniformBuffers;
			std::vector< VkSampler >		_samplers;
			std::vector< Texture * >		_textures;
			VulkanInstance *				_instance;
			VkDevice						_device;
			SwapChain *						_swapChain;
			RenderPass *					_renderPass;
	
			void		CreateDescriptorSetLayout(void);
			void		CreateTextureImage(void);
			void		CreateTextureSampler(void);
			void		CreateUniformBuffer(void);
			void		CreateDescriptorPool(void);
			void		CreateDescriptorSets(void);
			void		TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			VkShaderModule	createShaderModule(const std::vector<char>& code);
	
		public:
			Material(void);
			Material(const Material &);
			virtual ~Material(void);
	
			Material &	operator=(Material const & src);
	
			void	Initialize(SwapChain * swapchain, RenderPass * renderPass);
			void	CleanupGraphicPipeline(void) noexcept;
			void	CreateGraphicPipeline(void);
			void	BindDescriptorSets(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint);
			void	UpdateUniformBuffer(void);
	
			VkDescriptorPool	GetDescriptorPool(void) const;
			void	SetDescriptorPool(VkDescriptorPool tmp);
			
			VkPipelineLayout	GetGraphicPipelineLayout(void) const;
			void	SetGraphicPipelineLayout(VkPipelineLayout tmp);
			
			VkPipeline	GetGraphicPipeline(void) const;
			void	SetGraphicPipeline(VkPipeline tmp);
	};
	
	std::ostream &	operator<<(std::ostream & o, Material const & r);
}