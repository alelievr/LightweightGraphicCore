#pragma once

#include "IncludeDeps.hpp"
#include "Core/Components/Renderer.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		ComputeDispatcher : public Renderer
	{
		private:
			virtual void	Initialize(void) noexcept override;
			void			RecordDrawCommand(VkCommandBuffer cmd) noexcept override;
			void			CreateDescriptorSet(void) override;
			void			CreateDescriptorSetLayout(void) noexcept;

		public:
			ComputeDispatcher(void);
			ComputeDispatcher(const ComputeDispatcher &) = delete;
			virtual ~ComputeDispatcher(void);

			ComputeDispatcher &	operator=(ComputeDispatcher const & src) = delete;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r);
}
