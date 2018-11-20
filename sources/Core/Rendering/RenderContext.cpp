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

    const auto & set = renderComponents.find(componentType);

    if (set != renderComponents.end())
    {
        const std::unordered_set< Component * > comps = (*set).second;
        for (const auto & comp : comps)
            components.insert(dynamic_cast< T * >(comp));
    }
}

void    RenderContext::GetLights(std::unordered_set< Light * > & lights) 
{
    GetComponentSet< Light >(Light::type, lights);
}

void    RenderContext::GetMeshRenderers(std::unordered_set< MeshRenderer * > & meshRenderers) 
{
    GetComponentSet< MeshRenderer >(MeshRenderer::type, meshRenderers);
}