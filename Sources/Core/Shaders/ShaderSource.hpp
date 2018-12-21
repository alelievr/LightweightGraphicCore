#pragma once

#include <vector>
#include <string>
#include <stdint.h>

#include "IncludeDeps.hpp"
#include "Core/Shaders/ShaderBindingTable.hpp"

#include VULKAN_INCLUDE
#include GLFW_INCLUDE

namespace LWGC
{
	class		ShaderSource
	{
		private:
			struct ShaderFileInfo
			{
				std::string			path;
				long				lastModificationTime;
			};

			ShaderFileInfo			_sourceFile;
			VkShaderModule			_module;
			VkShaderStageFlagBits	_stage;
			std::vector< uint32_t >	_SpirVCode;

			uint32_t				_threadWidth;
			uint32_t				_threadHeight;
			uint32_t				_threadDepth;

			std::vector< char >		ReadFile(const std::string & fileName);
			long					GetFileModificationTime(const std::string & file) const;
			std::string				StageToText(const VkShaderStageFlagBits stage);

			const std::string		tmpFilePath = "/tmp/LWGC_spirV.tmp";

			static std::vector< std::string >	shaderIncludePaths;

			friend std::ostream &	operator<<(std::ostream & o, ShaderSource const & r);

		public:
			ShaderSource(void);
			ShaderSource(const ShaderSource &) = delete;
			ShaderSource(const std::string & shaderName, VkShaderStageFlagBits stage);
			virtual ~ShaderSource(void);

			ShaderSource &	operator=(ShaderSource const & src) = delete;

			void	SetSourceFile(const std::string & file, const VkShaderStageFlagBits stage);
			bool	NeedReload(void) const;
			void	Reload(void);
			void	Compile(void);
			void	GenerateBindingTable(ShaderBindingTable & bindingTable);

			VkShaderModule			GetModule(void) const;
			VkShaderStageFlagBits	GetStage(void) const;
			bool					HasSource(void) const;
			void					GetWorkingThreadSize(uint32_t & width, uint32_t & height, uint32_t & depth) const;

			static void	AddIncludePath(const std::string & path);
	};
}
