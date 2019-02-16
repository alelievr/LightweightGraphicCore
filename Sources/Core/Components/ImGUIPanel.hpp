#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		ImGUIPanel : public Object, public Component
	{
		private:
			ComponentIndex	_imGUIIndex;

		protected:
			bool			opened;

		public:
			ImGUIPanel(void);
			ImGUIPanel(const ImGUIPanel &) = delete;
			virtual ~ImGUIPanel(void);

			ImGUIPanel &	operator=(ImGUIPanel const & src) = delete;

			void OnEnable(void) noexcept override;
			void OnDisable(void) noexcept override;

			void Initialize(void) noexcept override;

			uint32_t	GetType(void) const noexcept override;

			virtual void DrawImGUI(void) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, ImGUIPanel const & r);
}
