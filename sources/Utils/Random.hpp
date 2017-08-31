#ifndef RANDOM_HPP
# define RANDOM_HPP
# include <iostream>
# include <string>

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

		static glm::vec2	InsideCircle;

		static glm::vec3	OnSphere(void);

		static glm::vec3	InsideSphere(void);

		static glm::quat	GetRotation(void);

		static Color	GetColor(void);

		static float	Range(const float a, const float b);

		static int	Range(const int a, const int b);

		static void	SetSeed(const long seed);
};

std::ostream &	operator<<(std::ostream & o, Random const & r);

#endif
