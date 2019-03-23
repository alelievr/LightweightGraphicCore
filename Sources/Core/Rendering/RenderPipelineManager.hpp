#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include "Core/Rendering/RenderPipeline.hpp"

namespace LWGC
{
	using BeginFrameRenderingDelegate = Delegate< void(void) >;
	using BeginCameraRenderingDelegate = Delegate< void(Camera *) >;
	using EndCameraRenderingDelegate = Delegate< void(Camera *) >;
	using EndFrameRenderingDelegate = Delegate< void(void) >;

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

			static BeginFrameRenderingDelegate		beginFrameRendering;
			static BeginCameraRenderingDelegate		beginCameraRendering;
			static EndCameraRenderingDelegate		endCameraRendering;
			static EndFrameRenderingDelegate		endFrameRendering;
	};
}
