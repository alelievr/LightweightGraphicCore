#include "LWGE.hpp"

int			main(void)
{
	LWGE::Application			app;
	LWGE::RenderPipeline *		p = app.GetRenderPipeline();

	LWGE::Model *				m = ObjLoader.Load("test.obj");

	p->push_back(m);

	while (app.ShouldNotQuit())
	{
		app.Update();
	}
	return (0);
}
