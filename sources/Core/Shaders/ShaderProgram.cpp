#include "ShaderProgram.hpp"


ShaderProgram::ShaderProgram(void)
{
	std::cout << "Default constructor of ShaderProgram called" << std::endl;
	this->_id = ;
	this->_sources = ;
}

ShaderProgram::ShaderProgram(ShaderProgram const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

ShaderProgram::~ShaderProgram(void)
{
	std::cout << "Destructor of ShaderProgram called" << std::endl;
}

bool		ShaderProgram::CompileAndLink(void)
{
	
}

void		ShaderProgram::AddVertexSourceFile(const std::string file)
{
	
}

void		ShaderProgram::AddFragmentSourceFile(const std::string file)
{
	
}

void		ShaderProgram::AddVertexSource(const std::string source)
{
	
}

void		ShaderProgram::AddFragmentSource(const std::string source)
{
	
}

void		ShaderProgram::AddGeometrySourceFile(const std::string file)
{
	
}

void		ShaderProgram::AddGeometrySource(const std::string source)
{
	
}

void		ShaderProgram::Bind(void)
{
	
}

void		ShaderProgram::Update(void)
{
	
}


ShaderProgram &	ShaderProgram::operator=(ShaderProgram const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_id = src.getId();
		this->_sources = src.getSources();
	}
	return (*this);
}

GLuint		ShaderProgram::GetId(void) const { return (this->_id); }
void		ShaderProgram::SetId(GLuint tmp) { this->_id = tmp; }

ShaderSource		ShaderProgram::GetSources(void) const { return (this->_sources); }
void		ShaderProgram::SetSources(ShaderSource tmp) { this->_sources = tmp; }

std::ostream &	operator<<(std::ostream & o, ShaderProgram const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
