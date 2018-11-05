#pragma once

#include "ChannelType.hpp"

namespace LWGC
{
	class	IChannel
	{
		public:
			virtual ChannelType		GetType(void) = 0;
			virtual int				Bind(void) = 0;
	};
}
