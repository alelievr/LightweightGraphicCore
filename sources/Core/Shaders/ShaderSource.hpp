#pragma once

#include <iostream>
#include <list>
#include <string>

#include "GLFW/glfw3.h"

namespace LWGC
{
	class		ShaderSource
	{
		private:
			struct ShaderFileInfo
			{
				std::string		path;
				long			lastModificationTime;
			};

			std::list< ShaderFileInfo >	_sourceFiles;


		public:
			ShaderSource(void);
			ShaderSource(const ShaderSource&);
			virtual ~ShaderSource(void);

			ShaderSource &	operator=(ShaderSource const & src);

			void	AddSourceFile(const std::string source);

			void	AddSource(const std::string source);

			bool	NeedReload(void);

			const GLchar **	GetVertexSources(int *nSources);

			const GLchar **	GetGeometrySources(int *nSources);

			const GLchar **	GetFragmentSources(int *nSources);
	};

	std::ostream &	operator<<(std::ostream & o, ShaderSource const & r);
}
