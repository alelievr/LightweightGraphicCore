#pragma once
#include <iostream>
#include <string>

#include "Core/GameObject.hpp"

namespace LWGC
{
	class		Light : IComponent
	{
		private:
	
	
		public:
			Light();
			Light(const Light&);
			virtual ~Light(void);
	
			Light &	operator=(Light const & src);
	
			virtual void	OnAdded(const GameObject & go) noexcept;
			virtual void	OnRemoved(const GameObject & go) noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, Light const & r);
}
