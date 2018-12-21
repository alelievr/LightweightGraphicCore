#pragma once

#include <iostream>
#include <string>
#include "Core/Vulkan/Material.hpp"

class		MaterialTable
{
	private:
		std::vector< std::shared_ptr< LWGC::Material > > _materials;

	public:
		MaterialTable();
		MaterialTable(const MaterialTable&) = delete;

		void	RegsiterMaterial(std::shared_ptr< LWGC::Material > material);
		void	DestroyMaterials();
		virtual ~MaterialTable(void);

		MaterialTable &	operator=(MaterialTable const & src) = delete;
};

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);
