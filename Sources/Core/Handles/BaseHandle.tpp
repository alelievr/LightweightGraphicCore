#pragma once

namespace LWGC::Handles
{
	template< typename T >
	class BaseHandle
	{
		public:
			BaseHandle(void) = default;
			BaseHandle(const BaseHandle & rhs) = delete;
			virtual ~BaseHandle(void) = default;

			BaseHandle & operator=(const BaseHandle & rhs) = delete;

			virtual bool	HasChanged(void) = 0;
			virtual T		GetDelta(void) = 0;
	};
}