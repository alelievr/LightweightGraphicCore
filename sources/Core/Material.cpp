#include "Material.hpp"


Material::Material(void)
{
	std::cout << "Default constructor of Material called" << std::endl;
	this->_renderQueueType = ;
	this->_renderFlags = 0;
	this->_shaderProgram = ;
}

Material::Material(Material const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Material::~Material(void)
{
	std::cout << "Destructor of Material called" << std::endl;
}

void		Material::Bind(void)
{
	
}

void		Material::SetColor(const std::string & name, const Color & c)
{
	
}

void		Material::SetFloat(const std::string & name, const float f)
{
	
}

void		Material::SetInt(const std::string & name, const int i)
{
	
}

void		Material::SetTexture(const Texture & t)
{
	
}

void		Material::SetMatrix(const std::string & name, const glm::mat4 & m)
{
	
}


Material &	Material::operator=(Material const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_renderQueueType = src.getRenderQueueType();
		this->_renderFlags = src.getRenderFlags();
		this->_shaderProgram = src.getShaderProgram();
	}
	return (*this);
}

RenderQueueType		Material::getRenderQueueType(void) const { return (this->_renderQueueType); }
void		Material::setRenderQueueType(RenderQueueType tmp) { this->_renderQueueType = tmp; }

int		Material::getRenderFlags(void) const { return (this->_renderFlags); }
void		Material::setRenderFlags(int tmp) { this->_renderFlags = tmp; }

ShaderProgram		Material::getShaderProgram(void) const { return (this->_shaderProgram); }
void		Material::setShaderProgram(ShaderProgram tmp) { this->_shaderProgram = tmp; }

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
