#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include "Core/Rendering/RenderPipeline.hpp"

namespace LWGC
{
	class		RenderPipelineManager
	{
		private:
			static std::unordered_set< RenderPipeline * >	_usedPipelines;

		public:
			RenderPipelineManager(void) = delete;
			~RenderPipelineManager(void) = delete;

			static RenderPipeline *		currentRenderPipeline;

			static void		SetCurrentRenderPipeline(RenderPipeline * newPipeline);
			static void		ReleaseAllPipelines(void);
	};
}
