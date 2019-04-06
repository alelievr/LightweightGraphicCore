#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"
#include "Core/Components/ImGUIPanel.hpp"
#include "Core/Profiler.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace LWGC
{
	class		ProfilerPanel : public ImGUIPanel
	{
		private:
			ProfilingSamples		_currentSample;
			std::vector< float >	_frameDurationHistory;

			const size_t		HISTORY_SIZE = 128;

		public:
			ProfilerPanel(void);
			ProfilerPanel(const ProfilerPanel &) = delete;
			virtual ~ProfilerPanel(void) = default;

			ProfilerPanel &	operator=(ProfilerPanel const & src) = delete;

			virtual void		DrawImGUI(void) noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, ProfilerPanel const & r);
}
