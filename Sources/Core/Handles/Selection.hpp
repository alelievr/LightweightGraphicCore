#pragma once

#include <iostream>
#include <string>

#include "Core/GameObject.hpp"
#include "Core/EventSystem.hpp"
#include "IncludeDeps.hpp"
#include "Core/Rendering/RenderContext.hpp"
#include "Utils/Bounds.hpp"

#include GLM_INCLUDE

namespace LWGC
{
	class		Selection
	{
		private:
			static GameObject *		_selectedGameObject;
			static GameObject *		_hoveredGameObject;
			static EventSystem *	_eventSystem;
			static RenderContext *	_renderContext;
			static glm::vec3		_worldRay;

			static void		Update(void) noexcept;
			static void		UpdateSelectedObject(Camera * cam) noexcept;
			static void		UpdateHandles(void) noexcept;
			static void		UpdateWorldRay(Camera * cam) noexcept;
			static void		MouseClickCallback(const glm::vec2 mousePos, int button, const ButtonAction action) noexcept;

		public:
			Selection(void) = delete;
			Selection(const Selection &) = delete;
			virtual ~Selection(void) = delete;

			static void			Initialize(void) noexcept;

			static GameObject *	GetSelectedGameObject(void) noexcept;
			static GameObject *	GetHoveredGameObject(void) noexcept;
			static glm::vec3	GetWorldRay(void) noexcept;

			Selection &	operator=(Selection const & src) = delete;
	};
}