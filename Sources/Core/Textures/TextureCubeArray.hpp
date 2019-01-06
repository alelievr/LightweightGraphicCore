#ifndef TEXTURECUBEARRAY_HPP
# define TEXTURECUBEARRAY_HPP
# include <iostream>
# include <string>

class		TextureCubeArray
{
	private:
		TextureCubeArray(void) = default;
		TextureCubeArray(const TextureCubeArray&);

	public:
		TextureCubeArray* Create();
		TextureCubeArray* Create(const TextureCubeArray&);
		virtual ~TextureCubeArray(void) = default;

		TextureCubeArray &	operator=(TextureCubeArray const & src);
};

std::ostream &	operator<<(std::ostream & o, TextureCubeArray const & r);

#endif
