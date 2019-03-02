#include "RenderPipelineManager.hpp"

using namespace LWGC;

std::vector< RenderPipeline * >	RenderPipelineManager::_usedPipelines;
RenderPipeline *				RenderPipelineManager::currentRenderPipeline = nullptr;

void		RenderPipelineManager::SetCurrentRenderPipeline(RenderPipeline * newPipeline)
{
	_usedPipelines.push_back(newPipeline);
	currentRenderPipeline = newPipeline;
}

void		RenderPipelineManager::ReleaseAllPipelines(void)
{
	for (auto pipeline : _usedPipelines)
		delete pipeline;
}