#include "ProfilerPanel.hpp"
#include "IncludeDeps.hpp"

#include "Core/Application.hpp"
#include "Core/Profiler.hpp"

#include IMGUI_INCLUDE

using namespace LWGC;

ProfilerPanel::ProfilerPanel() : ImGUIPanel()
{
	_frameDurationHistory.resize(HISTORY_SIZE, 0);

	// Get the last frame informations
	Application::update.AddListener([&](){
		_currentSample = Profiler::GetSamples();

		_frameDurationHistory.insert(_frameDurationHistory.begin(), Profiler::GetLastSample().duration);
		_frameDurationHistory.pop_back();
	});
}

void		ProfilerPanel::DrawImGUI(void) noexcept
{
	ImGui::Begin("Profiler");

	ImGui::PlotLines(
		"Frame time history",
		reinterpret_cast< float *>(_frameDurationHistory.data()),
		_frameDurationHistory.size(),
		0,
		NULL,
		0,
		FLT_MAX,
		ImVec2(300,80),
		sizeof(float)
	);

	for (const auto sample : _currentSample)
	{
		const auto & sampleName = sample.first;
		const auto & sampleData = sample.second;


		ImGui::Text("%s: %lf", sampleName.c_str(), sampleData.duration);
	}

	ImGui::End();
}

std::ostream &	operator<<(std::ostream & o, ProfilerPanel const & r)
{
	o << "ProfilerPanel" << std::endl;
	(void)r;
	return (o);
}
