#pragma once

#include <vector>
#include <string>
#include <stdint.h>

#include "IncludeDeps.hpp"

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

			std::vector< uint32_t >	ReadFile(const std::string & fileName);
			long					GetFileModificationTime(const std::string & file) const;
			std::string				StageToText(const VkShaderStageFlagBits stage);

			const std::string		tmpFilePath = "/tmp/LWGC_spirV.tmp";

			friend std::ostream &	operator<<(std::ostream & o, ShaderSource const & r);

		public:
			ShaderSource(void);
			ShaderSource(const ShaderSource &) = delete;
			virtual ~ShaderSource(void);

			ShaderSource &	operator=(ShaderSource const & src) = delete;

			void	SetSourceFile(const std::string file, const VkShaderStageFlagBits stage);
			bool	NeedReload(void) const;
			void	Reload(void);
			void	Compile(void);

			VkShaderModule			GetModule(void) const;
			VkShaderStageFlagBits	GetStage(void) const;
	};
}
