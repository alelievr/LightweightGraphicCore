#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	struct ShaderBinding
	{
		int					descriptorSet;
		int					bindingIndex;
		VkDescriptorType	descriptorType;
	};

	class		ShaderBindingTable
	{
		private:
			std::unordered_map< std::string, ShaderBinding >	bindings;

		public:
			ShaderBindingTable(void);
			ShaderBindingTable(const ShaderBindingTable&) = delete;
			virtual ~ShaderBindingTable(void);

			void		AddBinding(const std::string & name, int descriptorSet, int bindingIndex, VkDescriptorType descriptorType);
			void		GenerateSetLayouts();

			VkDescriptorSetLayout	GetDescriptorSetLayout(void);

			ShaderBindingTable &	operator=(ShaderBindingTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r);
}