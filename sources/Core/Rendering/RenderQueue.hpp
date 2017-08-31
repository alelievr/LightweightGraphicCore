#ifndef RENDERQUEUE_HPP
# define RENDERQUEUE_HPP
# include <iostream>
# include <string>

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

		void	SetRenderTraget(const RenderTarget & t);

		std::vector< IRenderable * >	getRenderList(void) const;
		void	setRenderList(std::vector< IRenderable * > tmp);
};

std::ostream &	operator<<(std::ostream & o, RenderQueue const & r);

#endif
