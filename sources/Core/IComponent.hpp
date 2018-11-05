#pragma once

namespace LWGC
{
	class IComponent
	{
		public:
			
			void OnComponentAdded(void) noexcept;
			void OnCompoenntRemoved(void) noexcept;
	}
}
