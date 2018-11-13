#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Rendering/SortingLayer.hpp"
#include "Core/Rendering/IRenderable.hpp"
#include "IComponent.hpp"

namespace LWGC
{
	class		MeshRenderer : public Object, public IRenderable, public IComponent
	{
		private:
			std::shared_ptr < Mesh >		_mesh;
			std::shared_ptr < Material >	_material;


		public:
			MeshRenderer(void);
			MeshRenderer(const MeshRenderer &);
			// TODO: the primitiveType constructor must be into Mesh.cpp
			// TODO: Add a constructor with a mesh and a material
			MeshRenderer(const PrimitiveType);
			virtual ~MeshRenderer(void);

			MeshRenderer &	operator=(MeshRenderer const & src);

			SortingLayer	GetSortingLayer(void) override;

			void	SetModel(const Mesh & mesh, const Material & material);
			void	SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material);

			Bounds	GetBounds(void) override;

			void	Render(void) override;

			void OnRemoved(const GameObject & go) noexcept override;
			void OnAdded(const GameObject & go) noexcept override;

			std::shared_ptr< Mesh >	GetMesh(void) const;
			void	SetMesh(std::shared_ptr< Mesh > tmp);
			
			std::shared_ptr< Material >	GetMaterial(void) const;
			void	SetMaterial(std::shared_ptr< Material > tmp);
	};

	std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r);
}
