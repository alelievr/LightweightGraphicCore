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
			uint32_t										_threadWidth;
			uint32_t										_threadHeight;
			uint32_t										_threadDepth;

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
			VkPipelineShaderStageCreateInfo *	GetShaderStages();
			void								GetWorkingThreadSize(uint32_t & width, uint32_t & height, uint32_t & depth);
			bool								HasBinding(const std::string & bindingName) const;
			const ShaderBindingTable *			GetShaderBindingTable(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r);
}
