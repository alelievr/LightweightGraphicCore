#include "ProfilerPanel.hpp"
#include "IncludeDeps.hpp"

#include "Core/Application.hpp"
#include "Core/Profiler.hpp"

#include IMGUI_INCLUDE

using namespace LWGC;

ProfilerPanel::ProfilerPanel() : ImGUIPanel()
{
	// Get the last frame informations
	Application::update.AddListener([&](){
		_currentSample = Profiler::GetSamples();
	});
}

void		ProfilerPanel::DrawImGUI(void) noexcept
{
	ImGui::Begin("Profiler");

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
