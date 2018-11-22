#pragma once

#include <unordered_map>
#include <unordered_set>

namespace LWGC
{
    class Light;
    class MeshRenderer;
    class Component;

    class RenderContext
    {
        public :
            RenderContext(void);
            RenderContext(const RenderContext & rc) = delete;
            virtual ~RenderContext(void);

            RenderContext operator=(const RenderContext & rhs) = delete;

            std::unordered_map< uint32_t, std::unordered_set< Component * > > renderComponents;

            template< class T >
            void    GetComponentSet(uint32_t componentType, std::unordered_set< T * > & components);
            void    GetLights(std::unordered_set< Light * > & lights);
            void    GetMeshRenderers(std::unordered_set< MeshRenderer * > & meshRenderers);
    };
}
