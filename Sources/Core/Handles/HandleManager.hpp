#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "Core/Handles/IHandleControl.hpp"
#include "Core/EventSystem.hpp"

namespace LWGC::Handles
{
	class		HandleManager
	{
		private:
			static std::unordered_set< IHandleControl * >		_handles;
			static IHandleControl *								_selectedHandle;
			static IHandleControl *								_hoveredHandle;

			static void		Update(void);

		public:
			HandleManager() = delete;
			HandleManager(const HandleManager&) = delete;
			virtual ~HandleManager(void) = delete;

			static void		Initialize(void);

			static void		MouseClickCallback(const glm::vec2 mousePos, int button, ButtonAction action);
			static void		UpdateHoveredHandle(void);

			HandleManager &	operator=(HandleManager const & src) = delete;
	};
}
