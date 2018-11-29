#pragma once

#include <iostream>
#include <string>

#include "Core/Texture.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include GLM_INCLUDE
#include "VulkanInstance.hpp"
#include "SwapChain.hpp"
#include "Vk.hpp"
#include "Core/Shaders/ShaderProgram.hpp"

#define PER_MATERIAL_BINDING_INDEX		3
#define TRILINEAR_CLAMP_BINDING_INDEX	1
#define ALBEDO_BINDING_INDEX			2

namespace LWGC
{
	class		Material
	{
		private:
			struct LWGC_PerMaterial
			{
				glm::vec4		albedo;
			};

			static VkDescriptorSetLayout	descriptorSetLayout;
			VkDescriptorSet					_descriptorSet;
			VkPipelineLayout				_graphicPipelineLayout;
			VkPipeline						_graphicPipeline;
			LWGC_PerMaterial				_perMaterial;
			UniformBuffer					_uniformPerMaterial;
			std::vector< VkSampler >		_samplers;
			std::vector< Texture * >		_textures;
			VulkanInstance *				_instance;
			VkDevice						_device;
			SwapChain *						_swapChain;
			RenderPass *					_renderPass;
			ShaderProgram *					_program;
	
			static void	CreateDescriptorSetLayout(void);
			void		CreateTextureImage(void);
			void		CreateTextureSampler(void);
			void		CreateUniformBuffer(void);
			void		CreateDescriptorSets(void);
			VkShaderModule	createShaderModule(const std::vector<char> & code);
	
		public:
			Material(void);
			Material(const Material &);
			Material(ShaderProgram * program);
			virtual ~Material(void);
	
			Material &	operator=(Material const & src);
	
			void	Initialize(SwapChain * swapchain, RenderPass * renderPass);
			void	CleanupGraphicPipeline(void) noexcept;
			void	CreateGraphicPipeline(void);
			void	BindDescriptorSets(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint);
			void	UpdateUniformBuffer(void);
	
			VkPipelineLayout	GetGraphicPipelineLayout(void) const;
			void				SetGraphicPipelineLayout(VkPipelineLayout tmp);
			
			VkPipeline			GetGraphicPipeline(void) const;
			void				SetGraphicPipeline(VkPipeline tmp);
			
			static VkDescriptorSetLayout	GetDescriptorSetLayout(void);
	};
	
	std::ostream &	operator<<(std::ostream & o, Material const & r);
}