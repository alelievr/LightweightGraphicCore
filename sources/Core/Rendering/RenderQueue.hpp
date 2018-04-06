#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IRenderable.hpp"

namespace LWGE
{
	class		RenderQueue
	{
		private:
			std::vector< IRenderable * >	_renderList;

		public:
			RenderQueue(void);
			RenderQueue(const RenderQueue&);
			virtual ~RenderQueue(void);

			RenderQueue &	operator=(RenderQueue const & src);

			void	RenderPass(void);

			void	SetRenderTraGet(const RenderTarget & t);

			std::vector< IRenderable * >	GetRenderList(void) const;
			void	SetRenderList(std::vector< IRenderable * > tmp);
	};

	std::ostream &	operator<<(std::ostream & o, RenderQueue const & r);
}
