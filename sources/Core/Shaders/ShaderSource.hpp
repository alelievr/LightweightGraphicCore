#pragma once

#include <vector>
#include <string>

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

			std::vector<char>	ReadFile(const std::string & fileName);
			long				GetFileModificationTime(const std::string & file) const;

			friend std::ostream &	operator<<(std::ostream & o, ShaderSource const & r);

		public:
			ShaderSource(void);
			ShaderSource(const ShaderSource &) = delete;
			virtual ~ShaderSource(void);

			ShaderSource &	operator=(ShaderSource const & src) = delete;

			void	SetSourceFile(const std::string file, const VkShaderStageFlagBits stage);
			void	SetSource(const std::vector< char > HLSLSource, const VkShaderStageFlagBits stage);
			bool	NeedReload(void) const;
			void	Reload(void);

			VkShaderModule	GetModule(void) const;
			VkShaderStageFlagBits	GetStage(void) const;
	};
}
