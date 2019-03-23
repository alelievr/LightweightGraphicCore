#include "RenderPipelineManager.hpp"

using namespace LWGC;

std::unordered_set< RenderPipeline * >	RenderPipelineManager::_usedPipelines;
RenderPipeline *						RenderPipelineManager::currentRenderPipeline = nullptr;

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