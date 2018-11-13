#include "LWGC.hpp"

void		ProcessEvent(LWGC::EventSystem * es, LWGC::Application & app)
{
	const LWGC::Event &	current = es->GetCurrentEvent();

	auto keyCode = current.GetKeyCode();

	switch (current.GetType())
	{
		case LWGC::EventType::KeyDown:
			if (keyCode == LWGC::KeyCode::A)
				std::cout << "key A pressed" << std::endl;
			if (keyCode == LWGC::KeyCode::ESCAPE)
				app.Quit();
			break ;

		default:
			break ;
	}
}

int			main(void)
{
	LWGC::Application		app;
	LWGC::EventSystem *		es = app.GetEventSystem();
	LWGC::Hierarchy *		hierarchy = app.GetHierarchy();

	//Initialize application
	app.Init();

	LWGC::IComponent * comp = new LWGC::MeshRenderer(LWGC::PrimitiveType::Cube);
	hierarchy->AddGameObject(new LWGC::GameObject(comp));

	//open window
	app.Open("Test Window", 1920, 1080, LWGC::WindowFlag::Resizable | LWGC::WindowFlag::Decorated | LWGC::WindowFlag::Focused);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}
