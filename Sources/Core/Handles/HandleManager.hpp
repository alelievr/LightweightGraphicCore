#pragma once

#include <iostream>
#include <string>

namespace LWGC
{
	class		HandleManager
	{
		private:
			static void		Update(void);

		public:
			HandleManager() = delete;
			HandleManager(const HandleManager&) = delete;
			virtual ~HandleManager(void) = delete;

			static void		Initialize(void);

			HandleManager &	operator=(HandleManager const & src) = delete;
	};
}
