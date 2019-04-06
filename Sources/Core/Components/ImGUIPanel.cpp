#include "ImGUIPanel.hpp"

#include "IncludeDeps.hpp"
#include "Core/Hierarchy.hpp"

#include IMGUI_INCLUDE

using namespace LWGC;

ImGUIPanel::ImGUIPanel(void) : opened(true), drawFunction(nullptr)
{
}

ImGUIPanel::ImGUIPanel(std::function< void(void) > drawFunction) : ImGUIPanel()
{
	SetDrawFunction(drawFunction);
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
	if (drawFunction != nullptr)
		drawFunction();
	else
	{
		ImGui::Begin("Panel", &opened, ImGuiWindowFlags_MenuBar);

		ImGui::Text("Hello World");
		ImGui::Text("Use ImGUIPanel::SetDrawFunction() to override this draw function");
		ImGui::Text("Or override ImGUIPanel::DrawImGUI to create a new component");

		ImGui::End();
	}
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

void			ImGUIPanel::SetDrawFunction(std::function< void(void) > drawFunction) noexcept
{
	this->drawFunction = drawFunction;
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
