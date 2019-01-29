#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"

#include GLM_INCLUDE

namespace LWGC
{
	class		Color
	{
		public:
			float	r;
			float	g;
			float	b;
			float	a;

			Color(void);
			Color(float r, float g, float b, float a = 1);
			Color(const Color&);
			virtual ~Color(void);

			Color &	operator=(Color const & src);
			//TODO: operators + - * / glm::vec4

			operator glm::vec4() const;

			Color	Lerp(const Color & b, const float t);

			static Color	HSVToRGB(const float H, const float S, const float V);
			static Color	Lerp(const Color & a, const Color & b, const float t);

			static Color	Black;
			static Color	White;
			static Color	Gray;
			static Color	LightGray;
			static Color	DarkRed;
			static Color	Red;
			static Color	DarkYellow;
			static Color	Yellow;
			static Color	DarkGreen;
			static Color	Green;
			static Color	DarkCyan;
			static Color	Cyan;
			static Color	DarkBlue;
			static Color	Blue;
			static Color	DarkMagenta;
			static Color	Magenta;
			static Color	Indigo;
			static Color	Violet;
			static Color	DarkBrown;
			static Color	Brown;
	};

	std::ostream &	operator<<(std::ostream & o, Color const & r);
}
