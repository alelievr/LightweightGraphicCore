#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "Core/Handles/IHandleControl.hpp"

namespace LWGC::Handles
{
	class		HandleManager
	{
		private:
			static std::unordered_set< IHandleControl * >		_handles;

			static void		Update(void);

		public:
			HandleManager() = delete;
			HandleManager(const HandleManager&) = delete;
			virtual ~HandleManager(void) = delete;

			static void		Initialize(void);

			HandleManager &	operator=(HandleManager const & src) = delete;
	};
}
