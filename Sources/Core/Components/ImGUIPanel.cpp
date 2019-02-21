#include "ImGUIPanel.hpp"

#include "IncludeDeps.hpp"
#include "Core/Hierarchy.hpp"

#include IMGUI_INCLUDE

using namespace LWGC;

ImGUIPanel::ImGUIPanel(void) : opened(true)
{
}

ImGUIPanel::~ImGUIPanel(void)
{
}

void			ImGUIPanel::Initialize(void) noexcept
{
	Component::Initialize();
}

void			ImGUIPanel::DrawImGUI(void) noexcept
{
	ImGui::Begin("Hello world !", &opened, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close Panel", "Ctrl+W"))
			{
				opened = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("Some text");

	ImGui::End();
}

void			ImGUIPanel::OnEnable() noexcept
{
	Component::OnEnable();
	_imGUIIndex = hierarchy->RegisterComponentInRenderContext(GetType(), this);
}

void			ImGUIPanel::OnDisable() noexcept
{
	Component::OnDisable();
	hierarchy->UnregisterComponentInRenderContext(GetType(), _imGUIIndex);
}

uint32_t		ImGUIPanel::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::ImGUIPanel);
}

std::ostream &	operator<<(std::ostream & o, ImGUIPanel const & r)
{
	o << "ImGUIPanel" << std::endl;
	(void)r;
	return (o);
}
