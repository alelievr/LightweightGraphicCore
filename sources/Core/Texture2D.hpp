#ifndef TEXTURE2D_HPP
# define TEXTURE2D_HPP
# include <iostream>
# include <string>

class		Texture2D
{
	private:


	public:
		Texture2D();
		Texture2D(const Texture2D&);
		virtual ~Texture2D(void);

		Texture2D &	operator=(Texture2D const & src);

		void	Apply(void);

		void	SetPixel(int x, int y);

		void	GetPixel(int x, int y);

		unsigned char *	GetRawData(void);

		void	SetRawData(unsigned char * data);
};

std::ostream &	operator<<(std::ostream & o, Texture2D const & r);

#endif
