#pragma once
#define GAMEOBJECT

#include <iostream>
#include <string>
#include <unordered_set>

#include "Transform.hpp"
#include "Object.hpp"
#include "Core/Components/IComponent.hpp"

namespace LWGC
{
	class		GameObject : public Object
	{
		private:
			Transform	_transform;
			bool		_active;
			std::unordered_set< IComponent * >	_components;

		public:
			GameObject(void);
			GameObject(const GameObject&);
			virtual		~GameObject(void);

			GameObject(IComponent * components);

			GameObject &	operator=(GameObject const & src);

			Transform	GetTransform(void) const;
			void		SetTransform(Transform tmp);

			IComponent *	AddComponent(IComponent * component) noexcept;
			void			RemoveComponent(IComponent * component) noexcept;

			void			SetActive(bool active);
			bool			IsActive(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, GameObject const & r);
}
