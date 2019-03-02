#include "NodeTree.hpp"

using namespace LWGC;

NodeTree::NodeTree(void)
{
	std::cout << "Default constructor of NodeTree called" << std::endl;
}

NodeTree::NodeTree(int width, int height) : _width(width), _height(height), _node(nullptr)
{
}

NodeTree::~NodeTree(void)
{
	std::cout << "Destructor of NodeTree called" << std::endl;
}


Rect	NodeTree::FindNode(int w, int h)
{
	if (_node == nullptr)
	{
		if (CreateStartNode(w, h) == nullptr)
			throw std::runtime_error("Texture is bigger than the texture2D Atlas");
		CreateNodes(_node);
		return (_node->area);
	}
	else if (_node->NodeRight && _node->NodeRight->area.CanFit(Rect(w, h, _node->area.GetMaxX, _node->area.GetMinY), _width, _height))
	{
		
	}
	else if (_node->NodeDown && _node->NodeDown->area.CanFit(Rect(w, h, _node->area.GetMinX, _node->area.GetMaxY), _width, _height))
	{
		
	}
}

Node	*NodeTree::CreateStartNode(int w, int h)
{
	if (w > _width || h > _height)
		return (nullptr);

	Rect area = Rect(w, h, 0, 0);
	Node *node = new Node{ area, true, nullptr, nullptr };

	return(node);
}

void	NodeTree::CreateNodes(Node *node)
{
	if (node->area.GetMaxX() < _width)
		node->NodeRight = new Node{ Rect(0, 0, static_cast<int>(node->area.GetMaxX()), static_cast<int>(node->area.GetMinY())), false, nullptr, nullptr };

	if (node->area.GetMaxY() < _height)
		node->NodeDown = new Node{ Rect(0, 0, static_cast<int>(node->area.GetMinX()), static_cast<int>(node->area.GetMaxY())), false, nullptr, nullptr };
}

void	NodeTree::Clear(void)
{

}