#pragma once

#include <iostream>
#include <string>

#include "Mesh.hpp"

namespace LWGE
{
	class		PrimitiveMeshFactory
	{
		private:

			static std::shared_ptr< Mesh >	_CreateCubeMesh(void);

		public:
			PrimitiveMeshFactory(void);
			PrimitiveMeshFactory(const PrimitiveMeshFactory&);
			virtual ~PrimitiveMeshFactory(void);

			PrimitiveMeshFactory &	operator=(PrimitiveMeshFactory const & src);

			static std::shared_ptr< Mesh >	CreateMesh(PrimitiveType type);
	};

	std::ostream &	operator<<(std::ostream & o, PrimitiveMeshFactory const & r);
}
