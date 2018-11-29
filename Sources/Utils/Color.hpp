#pragma once

#include <iostream>
#include <string>

namespace LWGC
{
	class		Color
	{
		private:
			float	_r;
			float	_g;
			float	_b;
			float	_a;
			


		public:
			Color(void);
			Color(const Color&);
			virtual ~Color(void);

			Color &	operator=(Color const & src);
			//TODO: operators + - * / glm::vec4

			Color	Lerp(const Color & b, const float t);

			static Color	HSVToRGB(const float H, const float S, const float V);
			static Color	Lerp(const Color & a, const Color & b, const float t);

			float	GetR(void) const;
			void	SetR(float tmp);
			
			float	GetG(void) const;
			void	SetG(float tmp);
			
			float	GetB(void) const;
			void	SetB(float tmp);
			
			float	GetA(void) const;
			void	SetA(float tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Color const & r);
}
