class IRenderPipeline
{
	public:
		void	Render(void) = 0;
		void	SetRenderTarget(RenderTarget & target) = 0;
}
