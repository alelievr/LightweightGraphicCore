#ifndef DEFERREDRENDERPIPELINE_HPP
# define DEFERREDRENDERPIPELINE_HPP
# include <iostream>
# include <string>

class		DeferredRenderPipeline : public IRenderPipeline
{
	private:


	public:
		DeferredRenderPipeline();
		DeferredRenderPipeline(const DeferredRenderPipeline&);
		virtual ~DeferredRenderPipeline(void);

		DeferredRenderPipeline &	operator=(DeferredRenderPipeline const & src);

		void	Render(void);

		void	SetRenderTarget(RenderTarget & target);
};

std::ostream &	operator<<(std::ostream & o, DeferredRenderPipeline const & r);

#endif
