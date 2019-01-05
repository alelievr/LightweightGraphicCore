#pragma once

#include <iostream>
#include <string>

#include "Mesh.hpp"

namespace LWGC
{
	class		PrimitiveMeshFactory
	{
		private:

			static void		_ComputeFrustumPoints(float fovY, float aspect, float nearPlane, float farPlane, std::vector< glm::vec3 > & points);
			static void		_ComputeCubePoints(float size, std::vector< glm::vec3 > & points);

			static std::shared_ptr< Mesh >	_CreateCubeMesh(const std::vector< glm::vec3 > points);
			static std::shared_ptr< Mesh >	_CreateWireframeCubeMesh(const std::vector< glm::vec3 > points);
			static std::shared_ptr< Mesh >	_CreateCubeMesh(void);
			static std::shared_ptr< Mesh >	_CreateWireframeCubeMesh(void);
			static std::shared_ptr< Mesh >	_CreateQuadMesh(void);

		public:
			PrimitiveMeshFactory(void);
			PrimitiveMeshFactory(const PrimitiveMeshFactory&);
			virtual ~PrimitiveMeshFactory(void);

			PrimitiveMeshFactory &	operator=(PrimitiveMeshFactory const & src);

			static std::shared_ptr< Mesh >	CreateMesh(PrimitiveType type);
			static std::shared_ptr< Mesh >	CreateFrustum(float fovRad, float aspect, float nearPlane, float farPlane);
			static std::shared_ptr< Mesh >	CreateWireframeFrustum(float fovRad, float aspect, float nearPlane, float farPlane);
			static std::shared_ptr< Mesh >	CreateCircle(float radius, int vertices);
			static std::shared_ptr< Mesh >	CreateWireframeCircle(float radius, int vertices);
	};

	std::ostream &	operator<<(std::ostream & o, PrimitiveMeshFactory const & r);
}
