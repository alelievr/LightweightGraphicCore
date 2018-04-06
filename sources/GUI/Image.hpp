#pragma once

#include <iostream>
#include <string>
#include "GLFW/glfw3.h"

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

		std::string	GetFile(void) const;
		void	SetFile(std::string tmp);
		
		GLuint	GetId(void) const;
		void	SetId(GLuint tmp);
};

std::ostream &	operator<<(std::ostream & o, Image const & r);
