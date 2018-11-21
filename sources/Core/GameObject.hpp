#pragma once
#define GAMEOBJECT

#include <iostream>
#include <string>
#include <unordered_set>

#include "Transform.hpp"
#include "Object.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Hierarchy.hpp"

namespace LWGC
{
	class		GameObject : public Object
	{
		friend class Hierarchy;

		private:
			std::shared_ptr< Transform >	_transform;
			bool			_active;
			std::unordered_set< Component * >	_components;
			Hierarchy *		_hierarchy;
			bool			_initialized;

			void SetHierarchy(Hierarchy * hierarchy);
			void Initialize() noexcept;

		public:
			GameObject(void);
			GameObject(const GameObject&);
			virtual		~GameObject(void);

			GameObject(Component * components);

			GameObject &	operator=(GameObject const & src);

			Hierarchy *		GetHierarchy(void) const noexcept;

			std::shared_ptr< Transform >	GetTransform(void) const;

			Component *		AddComponent(Component * component) noexcept;
			void			RemoveComponent(Component * component) noexcept;
			Component *		GetComponent(void) noexcept;

			void			SetActive(bool active);
			bool			IsActive(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, GameObject const & r);
}
