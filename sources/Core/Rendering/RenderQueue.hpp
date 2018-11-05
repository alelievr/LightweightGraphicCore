#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IRenderable.hpp"
#include "RenderTarget.hpp"

namespace LWGC
{
	class		RenderQueue
	{
		private:
			std::vector< std::shared_ptr< IRenderable > >	_renderList;

		public:
			RenderQueue(void);
			RenderQueue(const RenderQueue&);
			virtual ~RenderQueue(void);

			RenderQueue &	operator=(RenderQueue const & src);

			void	RenderPass(void);

			void	AddToRender(std::shared_ptr< IRenderable > renderable);
			void	RemoveToRender(const IRenderable & renderable);
	};

	std::ostream &	operator<<(std::ostream & o, RenderQueue const & r);
}
