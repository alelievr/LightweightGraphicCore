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

	std::ostream &	operator<<(std::ostream & o, glm::vec3 const & r);
	std::ostream &	operator<<(std::ostream & o, const glm::vec4 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::mat4 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::mat3 & r);
	std::ostream &	operator<<(std::ostream & o, const glm::quat & r);
}