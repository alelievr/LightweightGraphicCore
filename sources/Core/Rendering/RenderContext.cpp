#include "RenderContext.hpp"

using namespace LWGC;

RenderContext::RenderContext()
{
}

RenderContext::~RenderContext()
{
}

template< class T >
void    RenderContext::GetComponentVector(uint32_t componentType, std::vector< T * > & components)
{
    components.clear();

    for (IComponent * comp : renderComponents[componentType])
        components.push_back(dynamic_cast< T * >(comp));
}

void    RenderContext::GetLights(std::vector< Light * > & lights)
{
    GetComponentVector< Light >((uint32_t)RenderComponentType::Light, lights);
}

void    RenderContext::GetMeshRenderers(std::vector< MeshRenderer * > & meshRenderers)
{
    GetComponentVector< MeshRenderer >((uint32_t)RenderComponentType::MeshRenderer, meshRenderers);
}