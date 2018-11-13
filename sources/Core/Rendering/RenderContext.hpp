#pragma once

#include <unordered_map>
#include <vector>

#include "Core/Components/IComponent.hpp"
#include "Core/Components/Light.hpp"
#include "Core/Components/MeshRenderer.hpp"

namespace LWGC
{
    enum class RenderComponentType
    {
        MeshRenderer,
        Light,
    };

    class RenderContext
    {
        public :
            RenderContext(void);
            RenderContext(const RenderContext & rc) = delete;
            virtual ~RenderContext(void);

            RenderContext operator=(const RenderContext & rhs) = delete;

            std::unordered_map< uint32_t, std::vector< IComponent * > > renderComponents;

            template< class T >
            void    GetComponentVector(uint32_t componentType, std::vector< T * > & components);
            void    GetLights(std::vector< Light * > & lights);
            void    GetMeshRenderers(std::vector< MeshRenderer * > & meshRenderers);
    };
}
