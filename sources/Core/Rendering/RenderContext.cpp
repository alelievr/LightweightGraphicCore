#include "RenderContext.hpp"

using namespace LWGC;

template< class T >
void    RenderContext::GetComponentVector(uint32_t componentType, std::vector< T * > & components)
{
    const auto & r = renderComponents[componentType];
    components.clear();
    components.insert(r.begin(), r.end());
}

void    RenderContext::GetLights(std::vector< Light * > & lights)
{
    GetComponentVector< Light >((uint32_t)RenderComponentType::Light, lights);
}

void    RenderContext::GetMeshRenderers(std::vector< MeshRenderer * > & meshRenderers)
{
    GetComponentVector< MeshRenderer >((uint32_t)RenderComponentType::MeshRenderer, meshRenderers);
}