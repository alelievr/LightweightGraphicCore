#ifndef FORWARDRENDERPIPELINE_HPP
# define FORWARDRENDERPIPELINE_HPP
# include <iostream>
# include <string>

class		ForwardRenderPipeline : public IRenderPipeline
{
	private:
		RenderQueue		_renderQueues[static_cast< int >(RenderQueueType::Count)];
		RenderTarget	_target;

	public:
		ForwardRenderPipeline(void);
		virtual ~ForwardRenderPipeline(void);

		override void Render(void);
};

#endif
