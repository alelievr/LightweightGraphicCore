#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "Transform.hpp"

namespace LWGC
{
	class		GameObject : public Object
	{
		private:
			Transform	_trasform;
			std::unordered_set< IComponent * >	_components;

		public:
			GameObject(void);
			GameObject(const GameObject&);
			virtual		~GameObject(void);

			GameObject &	operator=(GameObject const & src);

			Transform	GetTransform(void) const;
			void		SetTransform(Transform tmp);

			IComponent *	AddComponent(IComponent * component) noexcept;
			void			RemoveComponent(IComponent * component) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, GameObject const & r);
}
