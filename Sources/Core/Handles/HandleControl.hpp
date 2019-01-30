#pragma once

#include "Core/Components/Camera.hpp"
#include "Core/Delegate.tpp"
#include "IncludeDeps.hpp"
#include GLM_INCLUDE

namespace LWGC::Handle
{
	class HandleControl
	{
		public:
			HandleControl(void);
			HandleControl(const HandleControl & hadnle) = delete;
			virtual ~HandleControl(void) = default;

			HandleControl &	operator=(const HandleControl & rhs) = delete;

			Delegate< void(HandleControl *) >						onSelected;
			Delegate< void(HandleControl *, const glm::vec3) >		onDelta;
			Delegate< void(HandleControl *) >						onHover;
			Delegate< void(HandleControl *) >						onNormal;

			// Update called only when the handle is selected
			virtual void		UpdateSelected(void) = 0;
			virtual float		UpdateDistance(Camera * cam) = 0;

			void          		BindGameObject(GameObject * go);
			virtual void		Disable(void);
			virtual void		Enable(void);
	};
}