#include "LWGE.hpp"

void		ProcessEvent(LWGE::EventSystem & es, LWGE::Application & app)
{
	const LWGE::Event &	current = es.GetCurrentEvent();

	auto keyCode = current.GetKeyCode();

	std::cout << "Current: " << (int)current.GetKeyCode() << std::endl;

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
	LWGE::Application		app;
	LWGE::IRenderPipeline &	rp = app.GetRenderPipeline();
	LWGE::EventSystem &		es = app.GetEventSystem();

	LWGE::Model			testModel(LWGE::PrimitiveType::Cube);

	//Initalize OpenGL context
	app.Init();

	rp.PushToQueue(testModel, LWGE::RenderQueueType::Geometry);
	
	//open window
	app.Open("test window", 200, 200, LWGE::WindowFlag::Resizable | LWGE::WindowFlag::Decorated | LWGE::WindowFlag::Focused);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}
