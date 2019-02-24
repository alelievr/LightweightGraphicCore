#pragma once

#include <iostream>
#include <string>
#include "Core/Rendering/RenderPipeline.hpp"

namespace LWGC
{
	class		RenderPipelineManager
	{
		public:
			RenderPipelineManager(void) = delete;
			~RenderPipelineManager(void) = delete;

			static RenderPipeline *		currentRenderPipeline;

			// TODO: function to swicth the render pipeline
			// static void		SwitchRenderPipeline(RenderPipeline * newPipeline);
	};
}
