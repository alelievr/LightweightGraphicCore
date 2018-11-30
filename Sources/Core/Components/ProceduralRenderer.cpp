#include "ProceduralRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

ProceduralRenderer::ProceduralRenderer(void)
{
}

ProceduralRenderer::~ProceduralRenderer(void)
{
}

void		ProceduralRenderer::RecordDrawCommand(VkCommandBuffer cmd) noexcept
{
	vkCmdDraw(cmd, _verticeCount, _elementCount, 0, 0);
}

uint32_t					ProceduralRenderer::GetType(void) const noexcept { return static_cast< uint32_t >(ComponentType::ProceduralRenderer); }

int		ProceduralRenderer::GetVerticeCount(void) const noexcept { return _verticeCount; }
void	ProceduralRenderer::SetVerticeCount(int verticeCount) noexcept { _verticeCount = verticeCount; }

int		ProceduralRenderer::GetElementCount(void) const noexcept { return _elementCount; }
void	ProceduralRenderer::SetElementCount(int elementCount) noexcept { _elementCount = elementCount; }


std::ostream &	operator<<(std::ostream & o, ProceduralRenderer const & r)
{
	o << "ProceduralRenderer" << std::endl;
	(void)r;
	return (o);
}
