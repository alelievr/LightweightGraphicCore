#include "ShaderSource.hpp"


ShaderSource::ShaderSource(void)
{
	std::cout << "Default constructor of ShaderSource called" << std::endl;
	this->_sourceFiles = ;
}

ShaderSource::ShaderSource(ShaderSource const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

ShaderSource::~ShaderSource(void)
{
	std::cout << "Destructor of ShaderSource called" << std::endl;
}

void		ShaderSource::AddSourceFile(const std::string source)
{
	
}

void		ShaderSource::AddSource(const std::string source)
{
	
}

bool		ShaderSource::NeedReload(void)
{
	
}

const GLchar **		ShaderSource::GetVertexSources(int *nSources)
{
	
}

const GLchar **		ShaderSource::GetGeometrySources(int *nSources)
{
	
}

const GLchar **		ShaderSource::GetFragmentSources(int *nSources)
{
	
}


ShaderSource &	ShaderSource::operator=(ShaderSource const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_sourceFiles = src.GetSourceFiles();
	}
	return (*this);
}

std::list< ShaderFileInfo >		ShaderSource::GetSourceFiles(void) const { return (this->_sourceFiles); }
void		ShaderSource::SetSourceFiles(std::list< ShaderFileInfo > tmp) { this->_sourceFiles = tmp; }

std::ostream &	operator<<(std::ostream & o, ShaderSource const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
