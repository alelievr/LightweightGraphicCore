#pragma once

#include <iostream>
#include <string>

#include "Core/Texture.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include GLM_INCLUDE
#include "VulkanInstance.hpp"
#include "Vk.hpp"
#include "Core/Shaders/ShaderProgram.hpp"

#define TRILINEAR_CLAMP_BINDING_INDEX	1

namespace LWGC
{
	class		TextureBinding
	{
		public:
			static const std::string	Albedo;
			static const std::string	Normal;
			static const std::string	Height;
			static const std::string	Smoothness;
	};

	class		SamplerBinding
	{
		public:
			static const std::string	TrilinearClamp;
			static const std::string	TrilinearRepeat;
			static const std::string	NearestClamp;
			static const std::string	NearestRepeat;
			static const std::string	AnisotropicTrilinearClamp;
			static const std::string	DepthCompare;
	};

	class		LWGCBinding
	{
		public:
			static const std::string	Frame;
			static const std::string	Camera;
			static const std::string	Material;
			static const std::string	Object;
	};

	class SwapChain;
	class RenderPass;

	class		Material
	{
		private:
			struct LWGC_PerMaterial
			{
				glm::vec4		albedo;
			};

			VkDescriptorSet							_descriptorSet;
			VkPipelineLayout						_pipelineLayout;
			VkPipeline								_pipeline;
			LWGC_PerMaterial						_perMaterial;
			UniformBuffer							_uniformPerMaterial;
			std::vector< VkSampler >				_samplers;
			VulkanInstance *						_instance;
			VkDevice								_device;
			SwapChain *								_swapChain;
			RenderPass *							_renderPass;
			ShaderProgram *							_program;
			std::vector< VkDescriptorSetLayout >	_setLayouts;
	
			void		CreateTextureSampler(void);
			void		CreateUniformBuffer(void);
			void		CreateDescriptorSets(void);
			void		CreatePipelineLayout(void);
			void		CompileShaders(void);
			void		CreateGraphicPipeline(void);
			void		CreateComputePipeline(void);
	
		public:
			Material(void);
			Material(const Material &);
			Material(const std::string & shader, VkShaderStageFlagBits stage);
			Material(const std::string & fragmentShader);
			Material(ShaderProgram * program);
			virtual ~Material(void);
	
			Material &	operator=(Material const & src);
	
			void	Initialize(SwapChain * swapchain, RenderPass * renderPass);
			void	CleanupPipeline(void) noexcept;
			void	CreatePipeline(void);
			void	UpdateUniformBuffer(void);

			VkPipeline			GetPipeline(void) const;
			VkPipelineLayout	GetPipelineLayout(void) const;
			uint32_t			GetDescriptorSetBinding(const std::string & setName) const;
			VkDescriptorSet		GetDescriptorSet(void) const;

			void				SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType);
			void				SetTexture(const std::string & bindingName, const Texture & texture, VkImageLayout imageLayout, VkDescriptorType descriptorType);
			void				SetSampler(const std::string & bindingName, VkSampler sampler);


			// TMP:
			void				SetDescriptorSet(VkDescriptorSet set);
	};
	
	std::ostream &	operator<<(std::ostream & o, Material const & r);
}