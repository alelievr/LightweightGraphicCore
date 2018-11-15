#include "RenderContext.hpp"

#include "Core/Components/Light.hpp"
#include "Core/Components/MeshRenderer.hpp"

using namespace LWGC;

RenderContext::RenderContext()
{
}

RenderContext::~RenderContext()
{
}

template< class T >
void    RenderContext::GetComponentSet(uint32_t componentType, std::unordered_set< T * > & components)
{
    components.clear();

    for (Component * comp : renderComponents[componentType])
        components.insert(dynamic_cast< T * >(comp));
}

void    RenderContext::GetLights(std::unordered_set< Light * > & lights)
{
    GetComponentSet< Light >((uint32_t)RenderComponentType::Light, lights);
}

void    RenderContext::GetMeshRenderers(std::unordered_set< MeshRenderer * > & meshRenderers)
{
    GetComponentSet< MeshRenderer >((uint32_t)RenderComponentType::MeshRenderer, meshRenderers);
}