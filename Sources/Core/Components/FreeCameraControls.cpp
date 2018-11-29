#include "FreeCameraControls.hpp"

#include "Core/Events/EventSystem.hpp"
#include "Core/Application.hpp"

using namespace LWGC;

FreeCameraControls::FreeCameraControls(void)
{
}

FreeCameraControls::~FreeCameraControls(void)
{
}

void			FreeCameraControls::OnAdded(const GameObject & go) noexcept
{
	Component::OnAdded(go);
}

void			FreeCameraControls::OnRemoved(const GameObject & go) noexcept
{
	Component::OnRemoved(go);
}

void		FreeCameraControls::Initialize(void) noexcept
{
	Component::Initialize();
}

void		FreeCameraControls::OnEnable(void) noexcept
{
	Component::OnEnable();
}

void		FreeCameraControls::OnDisable(void) noexcept
{
	Component::OnDisable();
}

void		FreeCameraControls::Update(void) noexcept
{
	transform->Translate(glm::vec3(0.1, 0, 0));
}

uint32_t	FreeCameraControls::GetType(void) const noexcept
{
	return type;
}

std::ostream &	operator<<(std::ostream & o, FreeCameraControls const & r)
{
	o << "FreeCameraControls" << std::endl;
	(void)r;
	return (o);
}
