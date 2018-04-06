#include "LWGE.hpp"

void		ProcessEvent(EventSystem *es)
{
	Input *	i = es.GetInput();

	if (i->GetKeyDown(KeyCode.A))
		std::cout << "key A pressed" << std::endl;
}

int			main(void)
{
	LWGE::Application			app(LWGE::DeferredPipeline);
	LWGE::RenderPipeline *		rp = app.GetRenderPipeline();
	LWGE::EventSystem *			es = app.GetEventSystem();

	LWGE::Model *				testModel = ObjLoader.Load("test.obj");

	rp->Add(testModel);

	app.Init(); //Initalize OpenGL context
	
	app.Open("window", 2300, 1500, LWGE::FullScreen); //open window, ...

	es->EnableInputEvents();

	while (app.ShouldNotQuit())
	{
		processEvent(es);
		app.Update();
	}
	return (0);
}
