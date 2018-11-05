#include "Mesh.hpp"

#include "GLFW/glfw3.h"

using namespace LWGC;

Mesh::Mesh(void)
{
	glGenBuffers(6, _vbo);
	glGenVertexArrays(1, &_vao);
}

Mesh::Mesh(Mesh const & src)
{
	*this = src;
}

Mesh::~Mesh(void)
{
	glDeleteBuffers(6, _vbo);
	glDeleteVertexArrays(1, &_vao);
}

void	Mesh::AddVertex(float x, float y, float z)
{
	_vertices.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddVertex(const glm::vec3 & p)
{
	_vertices.push_back(p);
}

void	Mesh::AddColor(const Color & c)
{
	_colors.push_back(c);
}

void	Mesh::AddTriangle(int p1, int p2, int p3)
{
	_triangles.push_back(p1);
	_triangles.push_back(p2);
	_triangles.push_back(p3);
}

void	Mesh::AddTangent(float x, float y, float z)
{
	_tangents.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddTriangle(const glm::vec3 & t)
{
	_tangents.push_back(t);
}

void	Mesh::AddUv(float u, float v)
{
	_uvs.push_back(glm::vec2(u, v));
}

void	Mesh::AddUv(const glm::vec2 & uv)
{
	_uvs.push_back(uv);
}

void	Mesh::AddNormal(float x, float y, float z)
{
	_normals.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddNormal(const glm::vec3 & n)
{
	_normals.push_back(n);
}

Bounds		Mesh::GetBounds(void) const
{
	return _bounds;
}

void		Mesh::UploadDatas(void)
{
	std::cout << "12 == " << sizeof(glm::vec3) << std::endl;

	//Fill vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _normals.size(), _normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _uvs.size(), _uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _colors.size(), _colors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _tangents.size(), _tangents.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _triangles.size(), _triangles.data(), GL_STATIC_DRAW);

	//Bind vertex array
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
}

void		Mesh::RecalculateBounds(void)
{
	for (const auto & v : _vertices)
	{
		_bounds.Encapsulate(v);
	}
}

void		Mesh::Clear(void)
{
	_vertices.clear();
	_normals.clear();
	_uvs.clear();
	_colors.clear();
	_tangents.clear();
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
		this->_bounds = src._bounds;
	}
	return (*this);
}

std::vector< glm::vec3 >		Mesh::GetVertices(void) const { return (this->_vertices); }
void		Mesh::SetVertices(const std::vector< glm::vec3 > & tmp) { this->_vertices = tmp; }

std::vector< glm::vec3 >		Mesh::GetNormals(void) const { return (this->_normals); }
void		Mesh::SetNormals(const std::vector< glm::vec3 > & tmp) { this->_normals = tmp; }

std::vector< glm::vec2 >		Mesh::GetUvs(void) const { return (this->_uvs); }
void		Mesh::SetUvs(const std::vector< glm::vec2 > & tmp) { this->_uvs = tmp; }

std::vector< Color >		Mesh::GetColors(void) const { return (this->_colors); }
void		Mesh::SetColors(const std::vector< Color > & tmp) { this->_colors = tmp; }

std::vector< glm::vec3 >		Mesh::GetTangents(void) const { return (this->_tangents); }
void		Mesh::SetTangents(const std::vector< glm::vec3 > & tmp) { this->_tangents = tmp; }

std::vector< int >			Mesh::GetTriangles(void) const { return this->_triangles; }
void		Mesh::SetTriangles(const std::vector< int > & tmp) { this->_triangles = tmp; }

std::ostream &	operator<<(std::ostream & o, Mesh const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
