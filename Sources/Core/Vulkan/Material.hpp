#pragma once

#include <iostream>
#include <string>

#include "Core/Textures/Texture.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Core/Shaders/ShaderBindingTable.hpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Shaders/BuiltinShaders.hpp"
#include "Core/Shaders/ShaderProgram.hpp"
#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include GLM_INCLUDE
#include "VulkanInstance.hpp"
#include "Vk.hpp"

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
		friend class RenderPass; // For binding descriptor sets

		private:
			struct LWGC_PerMaterial
			{
				glm::vec4		albedo;
			};

			struct	DescriptorSet
			{
				VkDescriptorSet	set;
				std::string		name;
			};

			using SetTable = std::unordered_map< uint32_t, DescriptorSet >;

			VkPipelineLayout						_pipelineLayout;
			VkPipeline								_pipeline;
			LWGC_PerMaterial						_perMaterial;
			UniformBuffer							_uniformPerMaterial;
			std::vector< VkSampler >				_samplers;
			VulkanInstance *						_instance;
			VkDevice								_device;
			bool									_isReady;
			SwapChain *								_swapChain;
			RenderPass *							_renderPass;
			ShaderProgram *							_program;
			std::vector< VkDescriptorSetLayout >	_setLayouts;
			const ShaderBindingTable *				_bindingTable;
			SetTable								_setTable;
			VkPipelineVertexInputStateCreateInfo	_vertexInputState;
			VkPipelineInputAssemblyStateCreateInfo	_inputAssemblyState;
			VkPipelineDepthStencilStateCreateInfo	_depthStencilState;
			VkPipelineRasterizationStateCreateInfo	_rasterizationState;

			Material(void);
			Material(const std::string & shader, VkShaderStageFlagBits stage);
			Material(const std::string & fragmentShader, const std::string & vertexShader = BuiltinShaders::DefaultVertex);
			Material(ShaderProgram * program);

			void		CreateTextureSampler(void);
			void		CreateUniformBuffer(void);
			void		CreatePipelineLayout(void);
			void		CompileShaders(void);
			void		CreateGraphicPipeline(void);
			void		CreateComputePipeline(void);
			void		BindDescriptorSets(RenderPass * renderPass);
			void		SetupDefaultSettings(void);
			bool		DescriptorSetExists(const std::string & bindingName, bool silent);
			void		InitMaterialIfPossible(void);
			void		AllocateDescriptorSet(const std::string & bindingName);

		public:
			Material(const Material &);
			virtual ~Material(void);

			Material &	operator=(Material const & src);

			static Material	*Create(void);
			static Material	*Create(const std::string & shader, VkShaderStageFlagBits stage);
			static Material	*Create(const std::string & fragmentShader, const std::string & vertexShader = BuiltinShaders::DefaultVertex);
			static Material	*Create(ShaderProgram * program);

			void	Initialize(SwapChain * swapchain, RenderPass * renderPass);
			void	CleanupPipeline(void) noexcept;
			void	CreatePipeline(void);
			void	UpdateUniformBuffer(void);
			void	MarkAsReady(void) noexcept;

			VkPipeline			GetPipeline(void) const;
			VkPipelineLayout	GetPipelineLayout(void) const;
			uint32_t			GetDescriptorSetBinding(const std::string & setName) const;
			void				GetComputeWorkSize(uint32_t & width, uint32_t & height, uint32_t & depth) const;
			bool				IsCompute(void) const;
			bool				IsReady(void) const noexcept;

			void				SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType, bool silent = false);
			void				SetTexture(const std::string & bindingName, const Texture * texture, VkImageLayout imageLayout, VkDescriptorType descriptorType, bool silent = false);
			void				SetSampler(const std::string & bindingName, VkSampler sampler, bool silent = false);

			void				SetVertexInputState(VkPipelineVertexInputStateCreateInfo info);
			void				SetInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo info);
			void				SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo info);
			void				SetRasterizationState(VkPipelineRasterizationStateCreateInfo info);
	};

	std::ostream &	operator<<(std::ostream & o, Material const & r);
}