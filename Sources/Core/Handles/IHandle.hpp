#pragma once

namespace LWGC
{
	template< typename T >
	class IHandle
	{
		public:
			virtual bool	HasChanged(void) = 0;
			virtual T		GetDelta(void) = 0;
			virtual void	Select(void) = 0;
			virtual void	UnSelect(void) = 0;
	};
}