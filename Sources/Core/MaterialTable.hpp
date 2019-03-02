#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Core/Vulkan/Material.hpp"
#include "Core/ObjectTable.tpp"

namespace LWGC
{
	class		MaterialTable : public ObjectTable<Material>
	{
		friend class Material;
		friend class ShaderProgram;

		private:
			LWGC::SwapChain *														_swapChain;
			LWGC::RenderPass *														_renderPass;
			static std::unordered_map<ShaderProgram *, std::vector< Material * > >	_shadersPrograms;
			bool																	_initialized;

			void 	UpdateMaterial(ShaderProgram *shaderProgram) noexcept;
			void 	NotifyMaterialReady(Material * material);

		public:
			MaterialTable(void);
			MaterialTable(const MaterialTable&) = delete;
			virtual ~MaterialTable(void);

			void 	RegsiterObject(Material * material) override;
			void 	Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass * renderPipeline);
			void	RecreateAll(void);
			bool	IsInitialized(void) const noexcept;
			void	SetRenderPass(RenderPass * renderPass);

			MaterialTable &	operator=(MaterialTable const & src) = delete;

			static MaterialTable *	Get(void);
	};

	std::ostream &	operator<<(std::ostream & o, MaterialTable const & r);
}