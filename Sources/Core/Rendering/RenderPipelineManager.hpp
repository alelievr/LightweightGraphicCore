#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Core/Rendering/RenderPipeline.hpp"

namespace LWGC
{
	class		RenderPipelineManager
	{
		private:
			static std::vector< RenderPipeline * >	_usedPipelines;

		public:
			RenderPipelineManager(void) = delete;
			~RenderPipelineManager(void) = delete;

			static RenderPipeline *		currentRenderPipeline;

			static void		SetCurrentRenderPipeline(RenderPipeline * newPipeline);
			static void		ReleaseAllPipelines(void);
	};
}
