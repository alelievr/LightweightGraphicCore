#include "DeferredRenderPipeline.hpp"

using namespace LWGE;

DeferredRenderPipeline::DeferredRenderPipeline(void)
{
	std::cout << "Default constructor of DeferredRenderPipeline called" << std::endl;
}

DeferredRenderPipeline::DeferredRenderPipeline(DeferredRenderPipeline const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

DeferredRenderPipeline::~DeferredRenderPipeline(void)
{
	std::cout << "Destructor of DeferredRenderPipeline called" << std::endl;
}

void		DeferredRenderPipeline::Render(void)
{
	
}

void		DeferredRenderPipeline::SetRenderTarget(RenderTarget & target)
{
	
}


DeferredRenderPipeline &	DeferredRenderPipeline::operator=(DeferredRenderPipeline const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, DeferredRenderPipeline const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
