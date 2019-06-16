#include "NodeTree.hpp"

#include "LWGC.hpp"

using namespace LWGC;

NodeTree::NodeTree(int width, int height) : _width(width), _height(height), _node(nullptr)
{
	Rect area = Rect(0, 0, 0, 0);
	_node = new Node{ area, false, nullptr, nullptr};
}

NodeTree::~NodeTree(void)
{
}

Rect	NodeTree::Allocate(int w, int h)
{
	if (w > _width || h > _height)
		throw std::runtime_error("Texture is bigger than the texture2DAtlas: "
			+ std::to_string(w) + "/" + std::to_string(h) + " requested but "
			+ std::to_string(_width) + "/" + std::to_string(_height) + " available");

	Rect ret;
	Node *tmp = _node;

	ret = SearchNode(NULL, tmp, w, h, tmp->area.GetMaxX(), tmp->area.GetMinY());

	if (ret == Rect::Invalid)
		throw std::runtime_error("No space left in texture2DAtlas");

	return (ret);
}

Rect	NodeTree::SearchNode(Node* parent, Node* node, int w, int h, int X, int Y)
{
	if (!node->haveTexture)
	{
		if (node->area.CanFit(Rect(w, h, X, Y), _width, _height))
		{
			node->area = Rect(w, h, X, Y);
			node->haveTexture = true;
			CreateNodes(parent, node);
			return(node->area);
		}
	}
	else
	{
		Rect rect = FindNode(node, w, h);
		if (rect != Rect::Invalid)
			return (rect);
	}
	return (Rect::Invalid);
}

Rect	NodeTree::FindNode(Node *node, int w, int h)
{
	if (node->NodeRight != nullptr)
	{
		Rect rect = SearchNode(node, node->NodeRight, w, h, node->area.GetMaxX(), node->area.GetMinY());
		if (rect != Rect::Invalid)
			return (rect);
	}
	if (node->NodeDown != nullptr)
	{
		Rect rect = SearchNode(NULL, node->NodeDown, w, h, node->NodeDown->area.GetMinX(), node->NodeDown->area.GetMaxY());
		if (rect != Rect::Invalid)
			return (rect);
	}

	return (Rect::Invalid);
}

void	NodeTree::CreateNodes(Node* parent, Node *node)
{
	if (node->area.GetMaxX() < _width) {
		node->NodeRight = new Node{ Rect(0, 0, static_cast<int>(node->area.GetMaxX()), static_cast<int>(node->area.GetMinY())), false, nullptr, nullptr };
	}
	if (node->area.GetMaxY() < _height){
		if (parent != NULL && parent->area.GetOffset().x != node->area.GetOffset().x) {
			node->NodeDown = parent->NodeDown;
		}
		else {
			node->NodeDown = new Node{ Rect(0, 0, static_cast<int>(node->area.GetMinX()), static_cast<int>(node->area.GetMaxY())), false, nullptr, nullptr };
		}
	}
}

void	NodeTree::ClearTree(Node *node)
{
	if (node->NodeRight != nullptr)
		ClearTree(node->NodeRight);
	if (node->NodeDown != nullptr)
		ClearTree(node->NodeDown);
	if (!node->NodeRight && !node->NodeDown)
	{
		delete node;
		node = nullptr;
	}
}

void	NodeTree::Clear(void)
{
	if (_node)
		ClearTree(_node);
}
