#include "LWGE.hpp"

void		ProcessEvent(LWGE::EventSystem *es, LWGE::Application & app)
{
	Input *	i = es.GetInput();

	if (i->GetKeyDown(LWGE::KeyCode::A))
		std::cout << "key A pressed" << std::endl;
	if (i->GetKeyDown(LWGE::KeyCode::ESCAPE))
		app.Quit();
}

int			main(void)
{
	LWGE::Application			app;
	LWGE::IRenderPipeline *		rp = app.GetRenderPipeline();
	LWGE::EventSystem *			es = app.GetEventSystem();

	LWGE::Model *				testModel = ObjLoader.Load("test.obj");

	//Initalize OpenGL context
	app.Init();

	rp.PushOpaque(testModel);
	
	//open window
	app.Open("test window", 200, 200, LWGE::WindowFlag::Resizable | LWGE::WindowFlag::Decorated);

	while (app.ShouldNotQuit())
	{
		processEvent(es);
		app.Update();
	}
	return (0);
}
