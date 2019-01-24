#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include GLM_INCLUDE

namespace LWGC::Handle
{
	class		HandleUtils
	{
		private:

		public:
			HandleUtils(void) = delete;
			HandleUtils(const HandleUtils &) = delete;
			virtual ~HandleUtils(void) = delete;

			static float		DistanceToSegment(const glm::vec3 & segment0, const glm::vec3 & segment1, const glm::vec3 & point);
			static glm::vec3	ProjectPointLine(const glm::vec3 & segment0, const glm::vec3 & segment1, const glm::vec3 & point);

			HandleUtils &	operator=(HandleUtils const & src) = delete;
	};
}
