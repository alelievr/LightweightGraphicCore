#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "Core/Rendering/RenderContext.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/GameObject.hpp"
#include "Core/Components/Camera.hpp"

namespace LWGC
{
	class		Hierarchy
	{
		private:
			std::vector< GameObject * >			_gameObjects;
			RenderContext						_renderContext;
			std::unordered_set< Component * >	_components;
			std::vector< Camera * >				_cameras;
			bool								_initialized;

		public:
			Hierarchy(void);
			Hierarchy(const Hierarchy &) = delete;
			virtual		~Hierarchy(void);

			Hierarchy &	operator=(Hierarchy const & src) = delete;

			void					Initialize(void);
			void					AddGameObject(GameObject * gameObject);
			void					RemoveGameObject(GameObject * gameObject);
			const ComponentIndex	RegisterComponent(uint32_t componentType, Component * component) noexcept;
			void					UnregisterComponent(const ComponentIndex & index) noexcept;
			const ComponentIndex	RegisterComponentInRenderContext(uint32_t componentType, Component * component) noexcept;
			void					UnregisterComponentInRenderContext(uint32_t componentType, const ComponentIndex & index) noexcept;

			GameObject *			GetGameObject(int index);
			std::vector< Camera * >	GetCameras(void) noexcept;
			RenderContext &			GetRenderContext(void);
	};

	std::ostream &	operator<<(std::ostream & o, Hierarchy const & r);
}
