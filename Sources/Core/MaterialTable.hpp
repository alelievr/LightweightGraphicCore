#pragma once

#include <iostream>
#include <string>
#include "Core/Vulkan/Material.hpp"
#include "Core/ObjectTable.tpp"

namespace LWGC
{
	class		MaterialTable : public ObjectTable<Material>
	{
		friend class Material;

		private:
			LWGC::SwapChain *			_swapChain;
			LWGC::RenderPass *			_renderPass;

			void NotifyMaterialReady(Material * material);

		public:
			MaterialTable();
			MaterialTable(const MaterialTable&) = delete;
			virtual ~MaterialTable(void);

			void 	Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass * renderPipeline);

			MaterialTable &	operator=(MaterialTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);
}