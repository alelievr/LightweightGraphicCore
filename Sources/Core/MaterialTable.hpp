#pragma once

#include <iostream>
#include <string>
#include "Core/Vulkan/Material.hpp"

namespace LWGC
{
	class		MaterialTable
	{
		private:
			std::vector< Material * > _materials;

		public:
			MaterialTable();
			MaterialTable(const MaterialTable&) = delete;

			void	RegsiterMaterial(Material * material);
			void	DestroyMaterials();
			virtual ~MaterialTable(void);

			MaterialTable &	operator=(MaterialTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);
}