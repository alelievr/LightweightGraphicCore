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
			GameObject *		_selectedGameObject;
			GameObject *		_hoveredGameObject;
			EventSystem *		_eventSystem;
			RenderContext *		_renderContext;
			glm::vec3			_worldRay;

			void		Update(void) noexcept;
			void		UpdateSelectedObject(Camera * cam) noexcept;
			void		UpdateHandles(void) noexcept;
			void		UpdateWorldRay(Camera * cam) noexcept;
			void		MouseClickCallback(const glm::vec2 mousePos, int button, const ButtonAction action) noexcept;

		public:
			Selection();
			Selection(const Selection &) = delete;
			virtual ~Selection(void);

			GameObject *		GetSelectedGameObject(void) const noexcept;
			GameObject *		GetHoveredGameObject(void) const noexcept;
			glm::vec3			GetWorldRay(void) const noexcept;

			Selection &	operator=(Selection const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, Selection const & r);
}