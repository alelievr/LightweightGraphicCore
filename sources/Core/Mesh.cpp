#include "Mesh.hpp"

using namespace LWGE;

Mesh::Mesh(void)
{
	std::cout << "Default constructor of Mesh called" << std::endl;
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
		this->_vertices = src.GetVertices();
		this->_normals = src.GetNormals();
		this->_uvs = src.GetUvs();
		this->_colors = src.GetColors();
		this->_tangents = src.GetTangents();
	}
	return (*this);
}

std::vector< glm::vec3 >		Mesh::GetVertices(void) const { return (this->_vertices); }
void		Mesh::SetVertices(std::vector< glm::vec3 > tmp) { this->_vertices = tmp; }

std::vector< glm::vec3 >		Mesh::GetNormals(void) const { return (this->_normals); }
void		Mesh::SetNormals(std::vector< glm::vec3 > tmp) { this->_normals = tmp; }

std::vector< glm::vec2 >		Mesh::GetUvs(void) const { return (this->_uvs); }
void		Mesh::SetUvs(std::vector< glm::vec2 > tmp) { this->_uvs = tmp; }

std::vector< Color >		Mesh::GetColors(void) const { return (this->_colors); }
void		Mesh::SetColors(std::vector< Color > tmp) { this->_colors = tmp; }

std::vector< glm::vec3 >		Mesh::GetTangents(void) const { return (this->_tangents); }
void		Mesh::SetTangents(std::vector< glm::vec3 > tmp) { this->_tangents = tmp; }

std::ostream &	operator<<(std::ostream & o, Mesh const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
