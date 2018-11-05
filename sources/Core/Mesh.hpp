#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IncludeDeps.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include GLFW_INCLUDE

#include GLM_INCLUDE
#include "../Utils/Color.hpp"
#include "../Utils/Bounds.hpp"
#include "PrimitiveType.hpp"

namespace LWGC
{
	class		Mesh
	{
		private:
			std::vector< glm::vec3 >	_vertices;
			std::vector< glm::vec3 >	_normals;
			std::vector< glm::vec2 >	_uvs;
			std::vector< Color >		_colors;
			std::vector< glm::vec3 >	_tangents;
			std::vector< int >			_triangles;
			Bounds						_bounds;

			GLuint						_vao;
			GLuint						_vbo[6];

		public:
			Mesh(void);
			Mesh(const Mesh &);
			virtual ~Mesh(void);

			Mesh &	operator=(Mesh const & src);

			Bounds	GetBounds(void) const;

			void	AddVertex(float x, float y, float z);
			void	AddVertex(const glm::vec3 & p);

			void	AddColor(const Color & c);

			void	AddTriangle(int p1, int p2, int p3);

			void	AddTangent(float x, float y, float z);
			void	AddTriangle(const glm::vec3 & t);

			void	AddUv(float u, float v);
			void	AddUv(const glm::vec2 & uv);

			void	AddNormal(float x, float y, float z);
			void	AddNormal(const glm::vec3 & n);

			void	RecalculateBounds(void);
			void	UploadDatas(void);
			void	Clear(void);

			std::vector< glm::vec3 >	GetVertices(void) const;
			void	SetVertices(const std::vector< glm::vec3 > & tmp);
			
			std::vector< glm::vec3 >	GetNormals(void) const;
			void	SetNormals(const std::vector< glm::vec3 > & tmp);
			
			std::vector< glm::vec2 >	GetUvs(void) const;
			void	SetUvs(const std::vector< glm::vec2 > & tmp);
			
			std::vector< Color >	GetColors(void) const;
			void	SetColors(const std::vector< Color > & tmp);
			
			std::vector< glm::vec3 >	GetTangents(void) const;
			void	SetTangents(const std::vector< glm::vec3 > & tmp);

			std::vector< int >	GetTriangles(void) const;
			void	SetTriangles(const std::vector< int > & tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Mesh const & r);
}
