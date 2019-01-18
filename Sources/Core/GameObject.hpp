#pragma once
#define GAMEOBJECT

#include <iostream>
#include <string>
#include <unordered_set>

#include "Transform.hpp"
#include "Object.hpp"
#include "Core/Components/Component.hpp"
#include "Core/Delegate.tpp"

namespace LWGC
{
	class		GameObject : public Object
	{
		friend class Hierarchy;

		private:
			bool								_active;
			std::unordered_set< Component * >	_components;
			bool								_initialized;

			void 		SetHierarchy(Hierarchy * hierarchy);
			void		UpdateComponentsActiveStatus(void);

		protected:
			Transform *		transform;
			Hierarchy *		hierarchy;

			virtual void Initialize(void) noexcept;

		public:
			GameObject(void);
			GameObject(const GameObject &) = delete;
			virtual		~GameObject(void);

			GameObject(Component * components);

			GameObject &	operator=(GameObject const & src) = delete;

			Delegate< void(bool) >	onEnableChanged;

			Hierarchy *		GetHierarchy(void) const noexcept;
			Transform *		GetTransform(void) const;

			Component *		AddComponent(Component * component) noexcept;
			void			RemoveComponent(Component * component) noexcept;
			Component *		GetComponent(void) noexcept;

			void			SetActive(bool active);
			bool			IsActive(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, GameObject const & r);
}
