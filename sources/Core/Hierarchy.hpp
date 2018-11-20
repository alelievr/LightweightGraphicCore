#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "GameObject.hpp"
#include "Core/Rendering/RenderContext.hpp"
#include "Core/Vulkan/SwapChain.hpp"

namespace LWGC
{
	class		Hierarchy
	{
		private:
			std::vector< GameObject * >	_gameObjects;
			RenderContext				_renderContext;

		public:
			Hierarchy(void);
			Hierarchy(const Hierarchy &) = delete;
			virtual		~Hierarchy(void);

			Hierarchy &	operator=(Hierarchy const & src) = delete;

			void					Initialize(void);
			void					AddGameObject(GameObject * gameObject);
			void					RemoveGameObject(GameObject * gameObject);
			const ComponentIndex	RegisterComponentInRenderContext(int componentType, Component * component) noexcept;
			void					UnregisterComponentInRenderContext(int componentType, const ComponentIndex & index) noexcept;
			const ComponentIndex	RegisterComponentInRenderContext(RenderComponentType componentType, Component * component) noexcept;
			void					UnregisterComponentInRenderContext(RenderComponentType componentType, const ComponentIndex & index) noexcept;

			GameObject *			GetGameObject(int index);
			RenderContext &			GetRenderContext(void);
	};

	std::ostream &	operator<<(std::ostream & o, Hierarchy const & r);
}
