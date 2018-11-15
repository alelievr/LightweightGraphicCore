#pragma once
#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/Components/Component.hpp"

namespace LWGC
{
	class		Light : public Component
	{
		private:
	
	
		public:
			Light();
			Light(const Light &) = delete;
			virtual ~Light(void);
	
			Light &	operator=(Light const & src) = delete;
	
			virtual void	OnAdded(const GameObject & go) noexcept;
			virtual void	OnRemoved(const GameObject & go) noexcept;
	};
	
	std::ostream &	operator<<(std::ostream & o, Light const & r);
}
