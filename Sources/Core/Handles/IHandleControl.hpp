#pragma once

#include "Core/Components/Camera.hpp"
#include "Core/Delegate.tpp"
#include "IncludeDeps.hpp"
#include GLM_INCLUDE

namespace LWGC::Handles
{
	class IHandleControl
	{
		public:
			Delegate< void(IHandleControl *) >						onSelected;
			Delegate< void(IHandleControl *, const glm::vec3) >		onDelta;

			// Update called only when the handle is selected
			void			UpdateSelected(void);
			float			UpdateDistance(Camera * cam);
	};
}