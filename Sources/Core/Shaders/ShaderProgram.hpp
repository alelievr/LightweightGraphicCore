#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IncludeDeps.hpp"

#include GLFW_INCLUDE
#include "ShaderSource.hpp"
#include "Core/Shaders/ShaderBindingTable.hpp"

namespace LWGC
{
	class		ShaderProgram
	{
		private:
			std::vector< ShaderSource * >					_shaderSources;
			std::vector< VkPipelineShaderStageCreateInfo >	_shaderStages;
			ShaderBindingTable								_bindingTable;

		public:
			ShaderProgram(void);
			ShaderProgram(const std::string & fragmentShaderName, const std::string & vertexShaderName = "Shaders/DefaultVertex.hlsl");
			ShaderProgram(const ShaderProgram &) = delete;
			virtual ~ShaderProgram(void);

			ShaderProgram &	operator=(ShaderProgram const & src) = delete;

			void		CompileAndLink(void);

			void		Bind(void);
			bool		Update(void);
			bool		IsCompiled(void) const noexcept;
			bool		IsCompute(void) const noexcept;

			void								SetSourceFile(const std::string & file, VkShaderStageFlagBits stage);
			uint32_t							GetDescriptorSetBinding(const std::string & bindingName) const;
			uint32_t							GetDescriptorIndex(const std::string & setElementName) const;
			std::vector< VkDescriptorSetLayout >GetDescriptorSetLayouts(void) const;
			VkPipelineShaderStageCreateInfo *	GetShaderStages();
			VkDescriptorSetLayout				GetDescriptorSetLayout(const std::string & setElementName) const;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r);
}
