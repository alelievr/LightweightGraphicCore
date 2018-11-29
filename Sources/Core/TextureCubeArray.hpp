#ifndef TEXTURECUBEARRAY_HPP
# define TEXTURECUBEARRAY_HPP
# include <iostream>
# include <string>

class		TextureCubeArray
{
	private:


	public:
		TextureCubeArray();
		TextureCubeArray(const TextureCubeArray&);
		virtual ~TextureCubeArray(void);

		TextureCubeArray &	operator=(TextureCubeArray const & src);
};

std::ostream &	operator<<(std::ostream & o, TextureCubeArray const & r);

#endif
