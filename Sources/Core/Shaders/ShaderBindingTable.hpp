#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include SPIRV_CROSS_INCLUDE

namespace LWGC
{
	struct ShaderBinding
	{
		int						descriptorSet;
		int						bindingIndex;
		VkDescriptorType		descriptorType;
		uint32_t				elementSize;
	};

	struct PushConstantBinding
	{
		uint32_t		offset;
		uint32_t		size;
	};

	class		ShaderBindingTable
	{
		private:
			std::unordered_map< std::string, ShaderBinding >			_bindings;
			std::unordered_map< std::string, PushConstantBinding >		_pushConstants;
			std::unordered_map< std::string, VkDescriptorSetLayout >	_elementLayouts;
			VkShaderStageFlagBits										_stageFlags;
			std::vector< VkDescriptorSetLayout >						_descriptorSetLayout;

		public:
			ShaderBindingTable(void);
			ShaderBindingTable(const ShaderBindingTable&) = delete;
			virtual ~ShaderBindingTable(void);

			void			SetStage(VkShaderStageFlagBits stage);
			ShaderBinding &	AddBinding(const std::string & name, int descriptorSet, int bindingIndex, VkDescriptorType descriptorType);
			PushConstantBinding & AddPushConstant(const std::string & name, uint32_t offset, uint32_t size);
			void			GenerateSetLayouts(void);
			bool			HasBinding(const std::string & bindingName) const;

			const std::vector< VkDescriptorSetLayout > &	GetDescriptorSetLayouts(void) const;
			VkDescriptorSetLayout							GetDescriptorSetLayout(const std::string & setElementName) const;
			std::vector< std::string >						GetBindingNames() const;
			uint32_t										GetDescriptorSetBinding(const std::string & bindingName) const;
			uint32_t										GetDescriptorIndex(const std::string & bindingName) const;
			const std::vector< VkPushConstantRange >		GetPushConstants(VkShaderStageFlags stages) const;
			const PushConstantBinding 						GetPushConstant(const std::string & name) const;
			bool											HasPushConstant(const std::string & name) const;

			ShaderBindingTable &	operator=(ShaderBindingTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r);
}