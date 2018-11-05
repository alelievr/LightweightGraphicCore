#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "GameObject.hpp"

namespace LWGC
{
	class		Hierarchy
	{
		private:
			std::vector< GameObject * >	_gameObjects;

		public:
			Hierarchy(void);
			Hierarchy(const Hierarchy &) = delete;
			virtual		~Hierarchy(void);

			Hierarchy &	operator=(Hierarchy const & src) = delete;

			void AddGameObject(GameObject * gameObject);
			void RemoveGameObject(GameObject * gameObject);
	};

	std::ostream &	operator<<(std::ostream & o, Hierarchy const & r);
}
