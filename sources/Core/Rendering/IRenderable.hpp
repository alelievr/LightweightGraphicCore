#pragma once

#include "../Rendering/SortingLayer.hpp"
#include "../../Utils/Bounds.hpp"

namespace LWGE
{
	class IRenderable
	{
		public:
			virtual void			OnPreRender(void);
			virtual void			Render(void) = 0;
			virtual void			OnPostRender(void);

			virtual SortingLayer	GetSortingLayer(void) = 0;
			virtual Bounds			GetBounds(void) = 0;
	};
}
