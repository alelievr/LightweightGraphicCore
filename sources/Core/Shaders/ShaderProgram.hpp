#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"

#include GLFW_INCLUDE
#include "ShaderSource.hpp"

namespace LWGC
{
	class		ShaderProgram
	{
		private:
			GLuint	_id;
			ShaderSource	_sources;


		public:
			ShaderProgram(void);
			ShaderProgram(const ShaderProgram&);
			virtual ~ShaderProgram(void);

			ShaderProgram &	operator=(ShaderProgram const & src);

			bool	CompileAndLink(void);

			void	AddVertexSourceFile(const std::string file);

			void	AddFragmentSourceFile(const std::string file);

			void	AddVertexSource(const std::string source);

			void	AddFragmentSource(const std::string source);

			void	AddGeometrySourceFile(const std::string file);

			void	AddGeometrySource(const std::string source);

			void	Bind(void);

			void	Update(void);

			GLuint	GetId(void) const;
			void	SetId(GLuint tmp);
			
			ShaderSource	GetSources(void) const;
			void	SetSources(ShaderSource tmp);
	};

	std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r);
}