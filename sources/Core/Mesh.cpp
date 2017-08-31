#include "Mesh.hpp"


Mesh::Mesh(void)
{
	std::cout << "Default constructor of Mesh called" << std::endl;
	this->_vertices = ;
	this->_normals = ;
	this->_uvs = ;
	this->_colors = ;
	this->_tangents = ;
}

Mesh::Mesh(Mesh const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Mesh::~Mesh(void)
{
	std::cout << "Destructor of Mesh called" << std::endl;
}

Bounds		Mesh::GetBounds(void) const
{
	
}

void		Mesh::Update()
{
	
}

void		Mesh::Clear()
{
	
}


Mesh &	Mesh::operator=(Mesh const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_vertices = src.getVertices();
		this->_normals = src.getNormals();
		this->_uvs = src.getUvs();
		this->_colors = src.getColors();
		this->_tangents = src.getTangents();
	}
	return (*this);
}

std::vector< glm::vec3 >		Mesh::getVertices(void) const { return (this->_vertices); }
void		Mesh::setVertices(std::vector< glm::vec3 > tmp) { this->_vertices = tmp; }

std::vector< glm::vec3 >		Mesh::getNormals(void) const { return (this->_normals); }
void		Mesh::setNormals(std::vector< glm::vec3 > tmp) { this->_normals = tmp; }

std::vector< glm::vec2 >		Mesh::getUvs(void) const { return (this->_uvs); }
void		Mesh::setUvs(std::vector< glm::vec2 > tmp) { this->_uvs = tmp; }

std::vector< Color >		Mesh::getColors(void) const { return (this->_colors); }
void		Mesh::setColors(std::vector< Color > tmp) { this->_colors = tmp; }

std::vector< glm::vec3 >		Mesh::getTangents(void) const { return (this->_tangents); }
void		Mesh::setTangents(std::vector< glm::vec3 > tmp) { this->_tangents = tmp; }

std::ostream &	operator<<(std::ostream & o, Mesh const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
