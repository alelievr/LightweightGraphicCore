#include "Tools.hpp"

#include "Core/Application.hpp"

using namespace LWGC;

int		Tools::_toolIndex = static_cast< int >(BuiltinTools::Position);

int			Tools::GetCurrentToolIndex(void) { return _toolIndex; }

void		Tools::Initialize(void) noexcept
{
	Application::update.AddListener(Tools::Update);
}

void		Tools::Update(void) noexcept
{
	// todo: update transforms of the current selected tools with selected game object
}