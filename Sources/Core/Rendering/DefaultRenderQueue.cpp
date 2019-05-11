#include "DefaultRenderQueue.hpp"

#include <algorithm>

#include "RenderPipelineManager.hpp"

using namespace LWGC;

DefaultRenderQueue::DefaultRenderQueue(void)
{
	RenderPipelineManager::beginCameraRendering.AddListener([&](Camera * camera){
		SortRenderQueue(camera, _transparentObjects);
	});
}

void			DefaultRenderQueue::AddRenderer(Renderer * renderer) noexcept
{
	if (renderer->GetMaterial()->IsTransparent())
		_transparentObjects.push_back(renderer);
	else
		_opaqueObjects.push_back(renderer);
}

uint32_t		DefaultRenderQueue::GetQueueCount(void) noexcept
{
	return 2;
}

void			DefaultRenderQueue::SortRenderQueue(Camera * camera, std::vector< Renderer * > & queue)
{
	glm::vec3	cameraPos = camera->GetTransform()->GetPosition();

	std::stable_sort(queue.begin(), queue.end(), [&](const auto & a, const auto & b){
		float d1 = glm::distance(a->GetTransform()->GetPosition(), cameraPos);
		float d2 = glm::distance(b->GetTransform()->GetPosition(), cameraPos);
		return d1 < d2;
	});
}

std::vector< Renderer * >		DefaultRenderQueue::GetRenderersForQueue(uint32_t queueIndex) noexcept
{
	switch (queueIndex)
	{
		case 0:
			return _opaqueObjects;
		case 1:
			return _transparentObjects;
		default:
			std::cerr << "Can't access the renderQueue index " + std::to_string(queueIndex) << std::endl;
	}
}

std::ostream &	operator<<(std::ostream & o, DefaultRenderQueue const & r)
{
	o << "Default Render Queue" << std::endl;
	(void)r;
	return (o);
}
