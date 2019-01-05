#pragma once

#include <iostream>
#include <string>

#include "Mesh.hpp"

namespace LWGC
{
	class		PrimitiveMeshFactory
	{
		private:

			static std::shared_ptr< Mesh >	_CreateCubeMesh(
				const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3,
				const glm::vec3 p4, const glm::vec3 p5, const glm::vec3 p6, const glm::vec3 p7);
			static std::shared_ptr< Mesh >	_CreateCubeMesh(void);
			static std::shared_ptr< Mesh >	_CreateQuadMesh(void);

		public:
			PrimitiveMeshFactory(void);
			PrimitiveMeshFactory(const PrimitiveMeshFactory&);
			virtual ~PrimitiveMeshFactory(void);

			PrimitiveMeshFactory &	operator=(PrimitiveMeshFactory const & src);

			static std::shared_ptr< Mesh >	CreateMesh(PrimitiveType type);
			static std::shared_ptr< Mesh >	CreateFrustum(float fovRad, float aspect, float nearPlane, float farPlane);
	};

	std::ostream &	operator<<(std::ostream & o, PrimitiveMeshFactory const & r);
}
