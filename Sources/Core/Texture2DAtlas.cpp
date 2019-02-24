#include "Texture2DAtlas.hpp"

#include <vector>

using namespace LWGC;

Texture2DAtlas::Texture2DAtlas(int w, int h)
{
	width = w;
	height = h;
	// alloc_mem
}

Texture2DAtlas::~Texture2DAtlas(void)
{
	std::cout << "Destructor of Texture2DAtlas called" << std::endl;
}

void *Texture2DAtlas::fit(Texture texture, int width, int height)
{
	auto node = findnode(width, height);
	if (node != NULL)
	{
		//save texture in node;
		return node;
	}
}

void *Texture2DAtlas::findnode(int w, int h)
{
	// if (root.used)
	// 	return this.findNode(root.right, w, h) || this.findNode(root.down, w, h);
	// else 
	if ((width <= w) && (height <= h))
		return NULL; //mem_ptr;
	else
		return NULL;
}

std::ostream &	operator<<(std::ostream & o, Texture2DAtlas const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
