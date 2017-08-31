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

		std::vector< glm::vec3 >	getVertices(void) const;
		void	setVertices(std::vector< glm::vec3 > tmp);
		
		std::vector< glm::vec3 >	getNormals(void) const;
		void	setNormals(std::vector< glm::vec3 > tmp);
		
		std::vector< glm::vec2 >	getUvs(void) const;
		void	setUvs(std::vector< glm::vec2 > tmp);
		
		std::vector< Color >	getColors(void) const;
		void	setColors(std::vector< Color > tmp);
		
		std::vector< glm::vec3 >	getTangents(void) const;
		void	setTangents(std::vector< glm::vec3 > tmp);
};

std::ostream &	operator<<(std::ostream & o, Mesh const & r);

#endif
