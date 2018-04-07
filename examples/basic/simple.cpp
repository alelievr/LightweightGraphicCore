#include "LWGE.hpp"

void		ProcessEvent(const std::unique_ptr< LWGE::EventSystem > & es, LWGE::Application & app)
{
	LWGE::Event		current = es->GetCurrentEvent();

	auto keyCode = current.GetKeyCode();

	switch (current.GetType())
	{
		case LWGE::EventType::KeyDown:
			if (keyCode == LWGE::KeyCode::A)
				std::cout << "key A pressed" << std::endl;
			if (keyCode == LWGE::KeyCode::ESCAPE)
				app.Quit();
			break ;

		default:
			break ;
	}
}

int			main(void)
{
	LWGE::Application	app;
	auto				rp = app.GetRenderPipeline();
	const auto &		es = app.GetEventSystem();

	//LWGE::Model *				testModel = ObjLoader.Load("test.obj");
	LWGE::Model			testModel(LWGE::PrimitiveType::Cube);

	//Initalize OpenGL context
	app.Init();

	rp->PushToQueue(testModel, LWGE::RenderQueueType::Geometry);
	
	//open window
	app.Open("test window", 200, 200, LWGE::WindowFlag::Resizable | LWGE::WindowFlag::Decorated);

	while (app.ShouldNotQuit())
	{
		ProcessEvent(es, app);
		app.Update();
	}
	return (0);
}
