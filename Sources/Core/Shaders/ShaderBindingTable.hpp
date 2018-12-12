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

	class		ShaderBindingTable
	{
		private:
			std::unordered_map< std::string, ShaderBinding >	_bindings;
			VkShaderStageFlagBits								_stageFlags;
			std::vector< VkDescriptorSetLayout >				_descriptorSetLayout;

		public:
			ShaderBindingTable(void);
			ShaderBindingTable(const ShaderBindingTable&) = delete;
			virtual ~ShaderBindingTable(void);

			void			SetStage(VkShaderStageFlagBits stage);
			ShaderBinding &	AddBinding(const std::string & name, int descriptorSet, int bindingIndex, VkDescriptorType descriptorType);
			void			GenerateSetLayouts(void);
			uint32_t		GetDescriptorSetBinding(const std::string & bindingName);

			const std::vector< VkDescriptorSetLayout > &	GetDescriptorSetLayout(void) const;

			ShaderBindingTable &	operator=(ShaderBindingTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r);
}