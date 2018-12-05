#pragma once

#include <unordered_set>

#include "Core/Transform.hpp"
#include "IncludeDeps.hpp"
#include "Core/Delegate.hpp"
#include VULKAN_INCLUDE

namespace LWGC
{
	class GameObject;
	class Hierarchy;
	class Component;
	
    using ComponentIndex = std::unordered_set< Component * >::iterator;

	enum class	ComponentType : uint32_t
	{
		MeshRenderer,
		ProceduralRenderer,
		Light,
		Camera,
		FreeCameraControls,
		ComputeDispatcher,
		Count, // Note: this MUST be the last element of the enum
	};

	class Component
	{
		friend class GameObject;
	
		private:
			bool				oldState;
		
		protected:
			bool				enabled;
			const GameObject *	gameObject;
			Transform *			transform;
			Hierarchy *			hierarchy;
			ComponentIndex		index;
			VkDevice			device;
			DelegateIndex		updateIndex;

			// Called when the vulkan is finished to initialize
			virtual void		Initialize() noexcept;
			void				UpdateGameObjectActive() noexcept;

		public:
			Component(void);
			Component(const Component & comp) = delete;
			virtual ~Component(void);

			Component operator=(const Component & rhs) = delete;

			virtual void	OnAdded(const GameObject & go) noexcept;
			virtual void	OnRemoved(const GameObject & go) noexcept;
			virtual void	OnEnable() noexcept;
			virtual void	OnDisable() noexcept;
			virtual	void	Update() noexcept;
			bool			IsEnabled() noexcept;

			void			Enable() noexcept;
			void			Disable() noexcept;

			virtual uint32_t	GetType(void) const noexcept;
	};
}
