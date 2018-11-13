#ifndef TEXTURE2DARRAY_HPP
# define TEXTURE2DARRAY_HPP
# include <iostream>
# include <string>

class		Texture2DArray
{
	private:


	public:
		Texture2DArray();
		Texture2DArray(const Texture2DArray&);
		virtual ~Texture2DArray(void);

		Texture2DArray &	operator=(Texture2DArray const & src);
};

std::ostream &	operator<<(std::ostream & o, Texture2DArray const & r);

#endif
