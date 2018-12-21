#pragma once

#include <iostream>
#include <string>
#include <unordered_map> 
#include "Core/Shaders/ShaderProgram.hpp"

namespace LWGC
{ 
	class		ShaderCache
	{
		private:
			static std::unordered_map<std::string, ShaderProgram	 *> _shaders;

		public:

			ShaderCache(void) = delete;
			ShaderCache(const ShaderCache&) = delete;
			virtual ~ShaderCache(void) = delete;

			static ShaderProgram	*GetShader(const std::string & shader, const std::string & elem);
			static ShaderProgram	*GetShader(const std::string & shader, int elem);

			ShaderCache &	operator=(ShaderCache const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, ShaderCache const & r) = delete;
}
