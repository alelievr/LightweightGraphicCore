#pragma once
# include <iostream>
# include <string>

#include "IncludeDeps.hpp"
#include "Utils/Rect.hpp"

#include GLM_INCLUDE

namespace LWGC
{
	struct		Node {
		Rect		area;
		bool		haveTexture;
		struct Node *NodeRight;
		struct Node *NodeDown;
	};

	class		NodeTree
	{
		private:
			int		_width;
			int		_height;
			Node	*_node;

		public:
			NodeTree(void) = delete;
			NodeTree(const NodeTree&) = delete;
			NodeTree(int width, int height);
			virtual ~NodeTree(void);
			NodeTree &	operator=(NodeTree const & src) = delete;

			Rect	FindNode(Node *node, int w, int h);
			Rect	SearchNode(Node *node, int w, int h, int X, int Y);
			Rect	Allocate(int w, int h);
			void	CreateNodes(Node *node);
			void	Clear(void);
	};

	std::ostream &	operator<<(std::ostream & o, NodeTree const & r) = delete;
}
