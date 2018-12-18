#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Core/IUpdatePerCamera.hpp"

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
            std::vector< IUpdatePerCamera * >   _updatePerCamera;
        
        public:
            RenderContext(void);
            RenderContext(const RenderContext & rc) = delete;
            virtual ~RenderContext(void);

            RenderContext operator=(const RenderContext & rhs) = delete;

            std::unordered_map< uint32_t, std::unordered_set< Component * > > renderComponents;

            template< class T >
            void    GetComponentSet(uint32_t componentType, std::unordered_set< T * > & components);
            template< class T >
            void    GetComponentSet(std::unordered_set< T * > & components);
            void    GetLights(std::unordered_set< Light * > & lights);
            void    GetRenderers(std::unordered_set< Renderer * > & meshRenderers);
            void    GetComputeDispatchers(std::unordered_set< ComputeDispatcher * > & computeDispatchers);
            std::vector< IUpdatePerCamera * > &   GetUpdatePerCameras(void);
    };
}
