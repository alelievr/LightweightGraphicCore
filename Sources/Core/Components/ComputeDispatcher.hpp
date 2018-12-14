#pragma once

#include "IncludeDeps.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Texture2D.hpp"

#include <iostream>
#include <string>
#include <memory>

namespace LWGC
{
	class		ComputeDispatcher : public Component
	{
		private:
			std::shared_ptr< Material >	_material;
			int							_width;
			int							_height;
			int							_depth;
			VkCommandBuffer				_computeCommandBuffer;
			ComponentIndex				_renderContextIndex;
			
			//TMP:
			Texture2D *					_texture;

			virtual void	Initialize(void) noexcept override;
			void			OnEnable(void) noexcept override;
			void			OnDisable(void) noexcept override;
			void			RecordComputeCommand(VkCommandBuffer cmd) noexcept;
			void			CreateDescriptorSet(void);

		public:
			ComputeDispatcher(void) = delete;
			ComputeDispatcher(std::shared_ptr< Material > material, int width, int height, Texture2D * texture, int depth = 1);
			ComputeDispatcher(const ComputeDispatcher &) = delete;
			virtual ~ComputeDispatcher(void);

			ComputeDispatcher &	operator=(ComputeDispatcher const & src) = delete;

			std::shared_ptr< Material >	GetMaterial(void);
			VkCommandBuffer				GetCommandBuffer(void);

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r);
}
