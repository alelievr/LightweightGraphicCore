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

		protected:
			void	Render(const std::vector< Camera * > & cameras, RenderContext & context) override;

		public:
			ForwardRenderPipeline(void);
			virtual ~ForwardRenderPipeline(void);
	};
}
