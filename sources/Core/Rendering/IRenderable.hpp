#pragma once

#include "Core/Vulkan/Material.hpp"
#include "../../Utils/Bounds.hpp"

namespace LWGC
{
	class IRenderable
	{
		public:
			// virtual void			UpdateUnoforms(void) = 0;
			virtual Bounds			GetBounds(void) = 0;
			// virtual Material *		GetMaterial(void) = 0;
	};
}
