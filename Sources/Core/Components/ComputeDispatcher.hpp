#pragma once

#include "IncludeDeps.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Vulkan/Material.hpp"

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

			virtual void	Initialize(void) noexcept override;
			void			RecordComputeCommand(VkCommandBuffer cmd) noexcept;
			void			CreateDescriptorSet(void);

		public:
			ComputeDispatcher(void) = delete;
			ComputeDispatcher(std::shared_ptr< Material > material, int width, int height, int depth = 0);
			ComputeDispatcher(const ComputeDispatcher &) = delete;
			virtual ~ComputeDispatcher(void);

			ComputeDispatcher &	operator=(ComputeDispatcher const & src) = delete;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r);
}
