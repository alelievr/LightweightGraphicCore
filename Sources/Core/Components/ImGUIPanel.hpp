#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"
#include "Core/Rendering/RenderPipeline.hpp"

#include <iostream>
#include <string>
#include <functional>

namespace LWGC
{
	class		ImGUIPanel : public Object, public Component
	{
		private:
			ComponentIndex	_imGUIIndex;

		protected:
			friend class RenderPipeline;
			bool							opened;
			std::function< void(void) >		drawFunction;

			virtual void DrawImGUI(void) noexcept;

		public:
			ImGUIPanel(void);
			ImGUIPanel(std::function< void(void) > drawFunction);
			ImGUIPanel(const ImGUIPanel &) = delete;
			virtual ~ImGUIPanel(void);

			ImGUIPanel &	operator=(ImGUIPanel const & src) = delete;

			void OnEnable(void) noexcept override;
			void OnDisable(void) noexcept override;

			void Initialize(void) noexcept override;

			uint32_t	GetType(void) const noexcept override;

			void		SetDrawFunction(std::function< void(void) > drawFunction) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, ImGUIPanel const & r);
}
