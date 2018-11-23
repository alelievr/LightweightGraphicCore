#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Component.hpp"

namespace LWGC
{
	class		MeshRenderer : public Object, public Component
	{
		private:
			std::shared_ptr< Mesh >		_mesh;
			std::shared_ptr< Material >	_material;
			VkCommandBuffer				_drawCommandBuffer;
			ComponentIndex				_renderContextIndex;

			void		Initialize(void) noexcept override;
			void		RecordDrawCommandBuffer(void);

		public:
			MeshRenderer(void);
			MeshRenderer(const MeshRenderer &) = delete;
			// TODO: the primitiveType constructor must be into Mesh.cpp
			// TODO: Add a constructor with a mesh and a material
			MeshRenderer(const PrimitiveType primitiveType);
			virtual ~MeshRenderer(void);

			MeshRenderer &	operator=(MeshRenderer const & src);

			void	SetModel(const Mesh & mesh, const Material & material);
			void	SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material);

			Bounds	GetBounds(void);

			void	OnEnable(void) noexcept override;
			void	OnDisable(void) noexcept override;

			void	CleanupGraphicPipeline(void) noexcept;
			void	CreateGraphicPipeline(void) noexcept;

			std::shared_ptr< Mesh >	GetMesh(void) const;
			void	SetMesh(std::shared_ptr< Mesh > tmp);
			
			std::shared_ptr< Material >	GetMaterial(void) const;
			void	SetMaterial(std::shared_ptr< Material > tmp);

			VkCommandBuffer		GetDrawCommandBuffer(void) const;

			virtual uint32_t			GetType() const noexcept override;
			static const uint32_t		type = 0;
	};

	std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r);
}
