#pragma once

#include <iostream>
#include <string>

#include "VulkanRenderPipeline.hpp"

namespace LWGC
{
	class		ForwardRenderPipeline : public VulkanRenderPipeline
	{
		private:
			RenderTarget	_target;

		public:
			ForwardRenderPipeline(void);
			virtual ~ForwardRenderPipeline(void);

			void	Render(const std::vector< Camera * > & cameras, const RenderContext & context) override;
	};
}
