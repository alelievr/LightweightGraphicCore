#include "RenderQueue.hpp"

using namespace LWGC;

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
	for (const auto & r : _renderList)
	{
		//check if object have been destroyed
		if (r)
			r->Render();
	}

	std::remove_if(_renderList.begin(), _renderList.end(), [](const auto & r)
		{
			return !r;
		}
	);
}

void		RenderQueue::AddToRender(std::shared_ptr< IRenderable > renderable)
{
	_renderList.push_back(renderable);
}

RenderQueue &	RenderQueue::operator=(RenderQueue const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_renderList = src._renderList;
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream & o, RenderQueue const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
