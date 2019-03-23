#include "RenderPipelineManager.hpp"

using namespace LWGC;

std::unordered_set< RenderPipeline * >	RenderPipelineManager::_usedPipelines;
RenderPipeline *RenderPipelineManager::currentRenderPipeline = nullptr;
BeginFrameRenderingDelegate RenderPipelineManager::beginFrameRendering;
BeginCameraRenderingDelegate RenderPipelineManager::beginCameraRendering;
EndCameraRenderingDelegate RenderPipelineManager::endCameraRendering;
EndFrameRenderingDelegate RenderPipelineManager::endFrameRendering;

void		RenderPipelineManager::SetCurrentRenderPipeline(RenderPipeline * newPipeline)
{
	_usedPipelines.insert(newPipeline);
	currentRenderPipeline = newPipeline;
}

void		RenderPipelineManager::ReleaseAllPipelines(void)
{
	for (auto pipeline : _usedPipelines)
		delete pipeline;
}