#include "NodeTree.hpp"

#include "LWGC.hpp"

using namespace LWGC;

NodeTree::NodeTree(int width, int height) : _width(width), _height(height), _node(nullptr)
{
	Rect area = Rect(width, height, 0, 0);
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
	ret = SearchNode(tmp, w, h);
	if (ret == Rect::Invalid)
		throw std::runtime_error("No space left in texture2DAtlas");
	return (ret);
}

Rect	NodeTree::SearchNode(Node* node, int w, int h)
{
	if (!node->haveTexture)
	{
		if (w <= node->area.GetSize().x && h <= node->area.GetSize().y)
		{
			node->haveTexture = true;
			CreateNodes(node, w, h);
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
		Rect rect = SearchNode(node->NodeRight, w, h);
		if (rect != Rect::Invalid)
			return (rect);
	}
	if (node->NodeDown != nullptr)
	{
		Rect rect = SearchNode(node->NodeDown, w, h);
		if (rect != Rect::Invalid)
			return (rect);
	}

	return (Rect::Invalid);
}

void	NodeTree::CreateNodes(Node *node, int w, int h)
{
	node->NodeDown = new Node{ Rect(node->area.GetSize().x, node->area.GetSize().y - h, node->area.GetOffset().x, node->area.GetOffset().y + h), false, nullptr, nullptr};
	node->NodeRight = new Node{ Rect(node->area.GetSize().x - w, static_cast<float>(h), node->area.GetOffset().x + w, node->area.GetOffset().y), false, nullptr, nullptr};
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
