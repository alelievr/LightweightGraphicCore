#include "LWGE.hpp"

int			main(void)
{
	Application			app;
	RenderPipeline *	p = app.GetRenderPipeline();

	Model			m = ObjLoader.Load("test.obj");

	p->push_back(m);

	while (app.ShouldNotQuit())
	{
		app.Render();
	}
	return (0);
}
