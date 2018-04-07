#pragma once

#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Color.hpp"
#include "Random.hpp"

namespace LWGE
{
	class		Random
	{
		private:


		public:
			Random();
			Random(const Random&);
			virtual ~Random(void);

			Random &	operator=(Random const & src);

			static float	GetFloat(void);

			static int	GetInt(void);

			static glm::vec2	OnCircle(void);

			static glm::vec2	InsideCircle(void);

			static glm::vec3	OnSphere(void);

			static glm::vec3	InsideSphere(void);

			static glm::quat	GetRotation(void);

			static Color	GetColor(void);

			static float	Range(const float a, const float b);

			static int	Range(const int a, const int b);

			static void	SetSeed(const long seed);
	};

	std::ostream &	operator<<(std::ostream & o, Random const & r);
}
