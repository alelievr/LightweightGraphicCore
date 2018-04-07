#pragma once

#include "ChannelType.hpp"

namespace LWGE
{
	class	IChannel
	{
		public:
			virtual ChannelType		GetType(void) = 0;
			virtual int				Bind(void) = 0;
	};
}
