#include "Material.hpp"

using namespace LWGE;

Material::Material(void)
{
	std::cout << "Default constructor of Material called" << std::endl;
	this->_renderQueueType = RenderQueueType::Geometry;
	this->_renderFlags = 0;
	this->_shaderProgram = nullptr;
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
		this->_renderQueueType = src.GetRenderQueueType();
		this->_renderFlags = src.GetRenderFlags();
		this->_shaderProgram = src.GetShaderProgram();
	}
	return (*this);
}

RenderQueueType		Material::GetRenderQueueType(void) const { return (this->_renderQueueType); }
void		Material::SetRenderQueueType(RenderQueueType tmp) { this->_renderQueueType = tmp; }

int		Material::GetRenderFlags(void) const { return (this->_renderFlags); }
void		Material::SetRenderFlags(int tmp) { this->_renderFlags = tmp; }

ShaderProgram		Material::GetShaderProgram(void) const { return (this->_shaderProgram); }
void		Material::SetShaderProgram(ShaderProgram tmp) { this->_shaderProgram = tmp; }

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
