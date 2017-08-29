#ifndef IMAGE_HPP
# define IMAGE_HPP
# include <iostream>
# include <string>

class		Image
{
	private:
		std::string	_file;
		GLuint	_id;


	public:
		Image();
		Image(const Image&);
		virtual ~Image(void);

		Image &	operator=(Image const & src);

		std::string	getFile(void) const;
		void	setFile(std::string tmp);
		
		GLuint	getId(void) const;
		void	setId(GLuint tmp);
};

std::ostream &	operator<<(std::ostream & o, Image const & r);

#endif
