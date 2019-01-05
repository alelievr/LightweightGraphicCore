#pragma once

#include <iostream>
#include <string>
#include "Core/Vulkan/Material.hpp"

namespace LWGC
{
	class		MaterialTable
	{
		friend class Material;

		private:
			std::vector< Material * >	_materials;
			LWGC::SwapChain *			_swapChain;
			LWGC::RenderPass *			_renderPass;
			std::vector< Material >		_stagingMaterials;

			void NotifyMaterialReady(Material * material);

		public:
			MaterialTable();
			MaterialTable(const MaterialTable&) = delete;

			void 	Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass * renderPipeline);
			void	RegsiterMaterial(Material * material);
			void	DestroyMaterials();
			virtual ~MaterialTable(void);

			MaterialTable &	operator=(MaterialTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);
}