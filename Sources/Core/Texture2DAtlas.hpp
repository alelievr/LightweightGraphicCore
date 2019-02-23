#ifndef TEXTURE2DATLAS_HPP
# define TEXTURE2DATLAS_HPP
# include <iostream>
# include <string>

class		Texture2DAtlas
{
	private:


	public:
		Texture2DAtlas();
		Texture2DAtlas(const Texture2DAtlas&);
		virtual ~Texture2DAtlas(void);

		Texture2DAtlas &	operator=(Texture2DAtlas const & src);
};

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r);

#endif
