#include "ProceduralRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

ProceduralRenderer::ProceduralRenderer(Material * material, int verticeCount, int elementCount)
	: Renderer(material), _verticeCount(verticeCount), _elementCount(elementCount)
{
	// For procedural materials we don't need vertex input datas
	VkPipelineVertexInputStateCreateInfo	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	material->SetVertexInputState(vertexInputInfo);
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
