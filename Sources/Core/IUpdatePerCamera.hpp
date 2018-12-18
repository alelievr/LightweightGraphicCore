#pragma once

#include "Core/Components/Camera.hpp"

namespace LWGC
{
    class IUpdatePerCamera
    {
        public:
            virtual void       UpdatePerCamera(const Camera * camera) noexcept = 0;
    };
}