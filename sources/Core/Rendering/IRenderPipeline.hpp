#pragma once

#include "RenderTarget.hpp"

namespace LWGE
{
	class IRenderPipeline
	{
		public:
			virtual void	Render(void) = 0;
			virtual void	SetRenderTarGet(RenderTarget & target) = 0;
	};
}
