#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"
#include "Core/Components/ImGUIPanel.hpp"
#include "Core/Profiler.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		ProfilerPanel : public ImGUIPanel
	{
		private:
			ProfilingSamples	 _currentSample;

		public:
			ProfilerPanel(void);
			ProfilerPanel(const ProfilerPanel &) = delete;
			virtual ~ProfilerPanel(void) = default;

			ProfilerPanel &	operator=(ProfilerPanel const & src) = delete;

			virtual void		DrawImGUI(void) noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ProfilerPanel const & r);
}
