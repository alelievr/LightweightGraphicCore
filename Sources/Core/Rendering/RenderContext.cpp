#include "RenderContext.hpp"

#include "Core/Components/Light.hpp"
#include "Core/Components/Renderer.hpp"
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

// TODO: optimize !
template< class T >
void    RenderContext::GetComponentSet(std::unordered_set< T * > & components)
{
    components.clear();

    for (const auto & compSet : renderComponents)
    {
        for (const auto & comp : compSet.second)
        {
            T * d = dynamic_cast< T * >(comp);
            if (d != nullptr)
                components.insert(d);
        }
    }
}

void    RenderContext::GetLights(std::unordered_set< Light * > & lights)
{
    GetComponentSet< Light >(static_cast< uint32_t >(ComponentType::Light), lights);
}

void    RenderContext::GetRenderers(std::unordered_set< Renderer * > & renderers)
{
    GetComponentSet< Renderer >(renderers);
}

void    RenderContext::GetComputeDispatchers(std::unordered_set< ComputeDispatcher * > & computeDispatchers)
{
    GetComponentSet< ComputeDispatcher >(static_cast< uint32_t >(ComponentType::ComputeDispatcher), computeDispatchers);
}

std::vector< IUpdatePerCamera * > &		RenderContext::GetUpdatePerCameras(void)
{
    return _updatePerCamera;
}