#ifndef MESH_HPP
# define MESH_HPP
# include <iostream>
# include <string>

class		Mesh
{
	private:
		std::vector< glm::vec3 >	_vertices;
		std::vector< glm::vec3 >	_normals;
		std::vector< glm::vec2 >	_uvs;
		std::vector< Color >	_colors;
		std::vector< glm::vec3 >	_tangents;


	public:
		Mesh();
		Mesh(const Mesh&);
		virtual ~Mesh(void);

		Mesh &	operator=(Mesh const & src);

		Bounds	GetBounds(void) const;

		void	Update();

		void	Clear();

		std::vector< glm::vec3 >	GetVertices(void) const;
		void	SetVertices(std::vector< glm::vec3 > tmp);
		
		std::vector< glm::vec3 >	GetNormals(void) const;
		void	SetNormals(std::vector< glm::vec3 > tmp);
		
		std::vector< glm::vec2 >	GetUvs(void) const;
		void	SetUvs(std::vector< glm::vec2 > tmp);
		
		std::vector< Color >	GetColors(void) const;
		void	SetColors(std::vector< Color > tmp);
		
		std::vector< glm::vec3 >	GetTangents(void) const;
		void	SetTangents(std::vector< glm::vec3 > tmp);
};

std::ostream &	operator<<(std::ostream & o, Mesh const & r);

#endif
