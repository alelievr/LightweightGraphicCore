#pragma once

#include <iostream>
#include <string>

#include "RenderPipeline.hpp"

namespace LWGC
{
	class		ForwardRenderPipeline : public RenderPipeline
	{
		private:
			RenderTarget	_target;

		protected:
			void	Render(const std::vector< Camera * > & cameras, RenderContext * context) override;

		public:
			ForwardRenderPipeline(void);
			virtual ~ForwardRenderPipeline(void);
	};
}
