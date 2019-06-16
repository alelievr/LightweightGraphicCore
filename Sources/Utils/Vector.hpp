#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include GLM_INCLUDE

namespace LWGC
{
	class		Vector
	{
		private:


		public:
			Vector(void) = delete;
			Vector(const Vector &) = delete;
			virtual ~Vector(void);

			Vector &	operator=(Vector const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, const glm::vec2 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::vec3 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::vec4 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::mat4 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::mat3 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::quat & r);
	std::ostream &	operator<<(std::ostream & o, const glm::ivec2 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::ivec3 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::ivec4 & r);
}