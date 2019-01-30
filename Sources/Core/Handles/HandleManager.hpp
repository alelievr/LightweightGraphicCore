#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "Core/Handles/HandleControl.hpp"
#include "Core/EventSystem.hpp"

namespace LWGC::Handle
{
	class		HandleManager
	{
		friend class HandleControl;

		private:
			static std::unordered_set< HandleControl * >		_handles;
			static HandleControl *								_selectedHandle;
			static HandleControl *								_hoveredHandle;

			static void		AddHandleControl(HandleControl * control);
			static void		RemoveHandleControl(HandleControl * control);
			static void		Update(void);

		public:
			HandleManager(void) = delete;
			HandleManager(const HandleManager&) = delete;
			virtual ~HandleManager(void) = delete;

			static void		Initialize(void);

			static void		MouseClickCallback(const glm::vec2 mousePos, int button, ButtonAction action);
			static void		UpdateHoveredHandle(void);

			HandleManager &	operator=(HandleManager const & src) = delete;
	};
}
