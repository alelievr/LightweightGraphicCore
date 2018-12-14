#include "RenderContext.hpp"

#include "Core/Components/Light.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Components/ComputeDispatcher.hpp"
#include "Core/Components/Component.hpp"

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
    GetComponentSet< Light >(static_cast< uint32_t >(ComponentType::Light), lights);
}

void    RenderContext::GetMeshRenderers(std::unordered_set< MeshRenderer * > & meshRenderers) 
{
    GetComponentSet< MeshRenderer >(static_cast< uint32_t >(ComponentType::MeshRenderer), meshRenderers);
}

void    RenderContext::GetComputeDispatchers(std::unordered_set< ComputeDispatcher * > & computeDispatchers)
{
    GetComponentSet< ComputeDispatcher >(static_cast< uint32_t >(ComponentType::ComputeDispatcher), computeDispatchers);
}