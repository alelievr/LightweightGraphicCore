#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Core/IUpdatePerCamera.hpp"

#include "Core/Components/Light.hpp"
#include "Core/Components/Renderer.hpp"
#include "Core/Components/ComputeDispatcher.hpp"
#include "Core/Components/Component.hpp"

namespace LWGC
{
    class Light;
    class Renderer;
    class Component;
    class ComputeDispatcher;

    class RenderContext
    {
        friend class Hierarchy;

        private:
            std::vector<IUpdatePerCamera *> _updatePerCamera;

        public:
            RenderContext(void);
            RenderContext(const RenderContext &rc) = delete;
            virtual ~RenderContext(void);

            RenderContext operator=(const RenderContext &rhs) = delete;

            std::unordered_map<uint32_t, std::unordered_set<Component *>> renderComponents;

            template< class T >
            void GetComponentSet(uint32_t componentType, std::unordered_set< T > &components)
            {
                components.clear();

                const auto &set = renderComponents.find(componentType);

                if (set != renderComponents.end())
                {
                    const std::unordered_set< Component * > comps = (*set).second;
                    for (const auto &comp : comps)
                        components.insert(dynamic_cast< T >(comp));
                }
            }

            template< class T >
            void GetComponentSet(std::unordered_set< T > &components)
            {
                components.clear();

                for (const auto &compSet : renderComponents)
                {
                    for (const auto &comp : compSet.second)
                    {
                        T d = dynamic_cast<T>(comp);
                        if (d != nullptr)
                            components.insert(d);
                    }
                }
            }

            void GetLights(std::unordered_set< Light * > &lights) { return GetComponentSet< Light * >(static_cast< uint32_t >(ComponentType::Light), lights); }
            void GetRenderers(std::unordered_set< Renderer * > &renderers) { return GetComponentSet< Renderer * >(renderers); }
            void GetComputeDispatchers(std::unordered_set< ComputeDispatcher * > &computeDispatchers) { return GetComponentSet< ComputeDispatcher * >(static_cast< uint32_t >(ComponentType::ComputeDispatcher), computeDispatchers); }
            std::vector< IUpdatePerCamera * > &GetUpdatePerCameras(void) { return _updatePerCamera; }
    };
}
