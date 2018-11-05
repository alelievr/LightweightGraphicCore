#pragma once

#include <iostream>
#include <string>

#include "Object.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Rendering/SortingLayer.hpp"
#include "Rendering/IRenderable.hpp"

namespace LWGC
{
	class		Model : public Object, public IRenderable
	{
		private:
			std::shared_ptr < Mesh >		_mesh;
			std::shared_ptr < Material >	_material;


		public:
			Model(void);
			Model(const Model &);
			Model(const PrimitiveType);
			virtual ~Model(void);

			Model &	operator=(Model const & src);

			SortingLayer	GetSortingLayer(void);

			void	SetModel(const Mesh & mesh, const Material & material);
			void	SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material);

			Bounds	GetBounds(void);

			void	Render(void);

			std::shared_ptr< Mesh >	GetMesh(void) const;
			void	SetMesh(std::shared_ptr< Mesh > tmp);
			
			std::shared_ptr< Material >	GetMaterial(void) const;
			void	SetMaterial(std::shared_ptr< Material > tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Model const & r);
}
