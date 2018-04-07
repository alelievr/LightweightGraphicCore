#pragma once

#include "../Rendering/SortingLayer.hpp"
#include "../../Utils/Bounds.hpp"

namespace LWGE
{
	class IRenderable
	{
		public:
			virtual void			Render(void) = 0;

			virtual SortingLayer	GetSortingLayer(void) = 0;
			virtual Bounds			GetBounds(void) = 0;
	};
}
