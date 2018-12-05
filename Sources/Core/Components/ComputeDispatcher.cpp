#include "ComputeDispatcher.hpp"

using namespace LWGC;

ComputeDispatcher::ComputeDispatcher(void)
{
}

ComputeDispatcher::~ComputeDispatcher(void)
{
}

void	ComputeDispatcher::CreateDescriptorSet(void)
{

}

void	ComputeDispatcher::CreateDescriptorSetLayout(void) noexcept
{

}


void			ComputeDispatcher::Initialize(void) noexcept
{
	Component::Initialize();
}

uint32_t		ComputeDispatcher::GetType(void) const noexcept
{
	return static_cast< uint32_t >(ComponentType::ComputeDispatcher);
}

std::ostream &	operator<<(std::ostream & o, ComputeDispatcher const & r)
{
	o << "ComputeDispatcher" << std::endl;
	(void)r;
	return (o);
}
