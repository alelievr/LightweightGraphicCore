#include "LWGE.hpp"

void		ProcessEvent(EventSystem *es)
{
	Input *	i = es.GetInput();

	if (i->GetKeyDown(KeyCode.A))
		std::cout << "key A pressed" << std::endl;
}

int			main(void)
{
	LWGE::Application			app;
	LWGE::RenderPipeline *		rp = app.GetRenderPipeline();
	LWGE::EventSystem *			es = app.GetEventSystem();

	LWGE::Model *				testModel = ObjLoader.Load("test.obj");

	rp->Add(testModel);

	app.Start(); //Initalize OpenGL context, open window, ...

	es->EnableInputEvents();

	while (app.ShouldNotQuit())
	{
		processEvent(es);
		app.Update();
	}
	return (0);
}
