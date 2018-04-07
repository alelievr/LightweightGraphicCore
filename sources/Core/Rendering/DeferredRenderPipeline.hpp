#pragma once

#include <iostream>
#include <string>

#include "IRenderPipeline.hpp"

namespace LWGE
{
	class		DeferredRenderPipeline : public IRenderPipeline
	{
		private:


		public:
			DeferredRenderPipeline();
			DeferredRenderPipeline(const DeferredRenderPipeline&);
			virtual ~DeferredRenderPipeline(void);

			DeferredRenderPipeline &	operator=(DeferredRenderPipeline const & src);

			void	Render(void) override;
			void	SetRenderTarget(RenderTarget & target) override;
	};

	std::ostream &	operator<<(std::ostream & o, DeferredRenderPipeline const & r);
}
