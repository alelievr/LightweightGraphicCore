#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Component.hpp"
#include "Core/Components/Renderer.hpp"

namespace LWGC
{
	class		MeshRenderer : public Renderer
	{
		private:
			std::shared_ptr< Mesh >		_mesh;

			void		Initialize(void) noexcept override;
			void		RecordDrawCommand(VkCommandBuffer cmd) noexcept override;

		public:
			MeshRenderer(void);
			MeshRenderer(const MeshRenderer &) = delete;
			MeshRenderer(const PrimitiveType prim, std::shared_ptr< Material > material);
			// TODO: the primitiveType constructor must be into Mesh.cpp
			// TODO: Add a constructor with a mesh and a material
			MeshRenderer(const PrimitiveType primitiveType);
			virtual ~MeshRenderer(void);

			MeshRenderer &	operator=(MeshRenderer const & src) = delete;

			void	SetModel(const Mesh & mesh, const Material & material);
			void	SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material);

			std::shared_ptr< Mesh >	GetMesh(void) const;
			void	SetMesh(std::shared_ptr< Mesh > tmp);

			virtual uint32_t			GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r);
}
