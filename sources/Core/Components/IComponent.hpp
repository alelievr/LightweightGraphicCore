#pragma once

namespace LWGC
{
	class GameObject;

	class IComponent
	{
		public:
			
			virtual void OnAdded(const GameObject & go) noexcept = 0;
			virtual void OnRemoved(const GameObject & go) noexcept = 0;
	};
}

#include "GameObject.hpp"
