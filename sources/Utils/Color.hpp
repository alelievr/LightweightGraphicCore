#ifndef COLOR_HPP
# define COLOR_HPP
# include <iostream>
# include <string>

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

		static Color	HSVToRGB(const float H, const float S, const float V) const;
		static Color	Lerp(const Color & a, const Color & b, const float t);

		float	getR(void) const;
		void	setR(float tmp);
		
		float	getG(void) const;
		void	setG(float tmp);
		
		float	getB(void) const;
		void	setB(float tmp);
		
		float	getA(void) const;
		void	setA(float tmp);
};

std::ostream &	operator<<(std::ostream & o, Color const & r);

#endif
