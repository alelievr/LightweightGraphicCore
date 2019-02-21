#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"

#include GLM_INCLUDE
#include GLM_INCLUDE_QUATERNION
#include "Color.hpp"
#include "Random.hpp"

namespace LWGC
{
	class		Random
	{
		private:


		public:
			Random();
			Random(const Random&);
			virtual ~Random(void);

			Random &	operator=(Random const & src);

			static float		GetValue(void);
			static glm::vec2	OnCircle(void);
			static glm::vec2	InsideCircle(void);
			static glm::vec3	OnSphere(void);
			static glm::vec3	InsideSphere(void);
			static glm::quat	GetRotation(void);
			static glm::vec3	GetPosition(const glm::vec3 & min, const glm::vec3 & max);
			static Color		GetColor(void);
			static float		Range(const float a, const float b);
			static int			Range(const int a, const int b);
			static void	SetSeed(const long seed);
	};

	std::ostream &	operator<<(std::ostream & o, Random const & r);
}
