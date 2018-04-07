#include "RenderQueue.hpp"

using namespace LWGE;

RenderQueue::RenderQueue(void)
{
	std::cout << "Default constructor of RenderQueue called" << std::endl;
}

RenderQueue::RenderQueue(RenderQueue const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

RenderQueue::~RenderQueue(void)
{
	std::cout << "Destructor of RenderQueue called" << std::endl;
}

void		RenderQueue::RenderPass(void)
{
	
}

void		RenderQueue::SetRenderTraGet(const RenderTarget & t)
{
	
}


RenderQueue &	RenderQueue::operator=(RenderQueue const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_renderList = src.GetRenderList();
	}
	return (*this);
}

std::vector< IRenderable * >		RenderQueue::GetRenderList(void) const { return (this->_renderList); }
void		RenderQueue::SetRenderList(std::vector< IRenderable * > tmp) { this->_renderList = tmp; }

std::ostream &	operator<<(std::ostream & o, RenderQueue const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
