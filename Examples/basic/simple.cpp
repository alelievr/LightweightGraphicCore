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

	LWGC::ShaderSource::AddIncludePath("../../");

	//Initialize application
	app.Init();

	auto testMat = std::make_shared< LWGC::Material >(LWGC::ShaderProgram::Standard);
	auto cube = new LWGC::GameObject(new LWGC::MeshRenderer(LWGC::PrimitiveType::Cube, testMat));
	auto cam = new LWGC::GameObject(new LWGC::Camera());
	cube->GetTransform()->Translate(glm::vec3(0, 1, 0));
	cam->GetTransform()->Translate(glm::vec3(0, 0, -5));
	cam->AddComponent(new LWGC::FreeCameraControls());
	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);

	//open window
	app.Open("Test Window", 1920, 1080, LWGC::WindowFlag::Resizable | LWGC::WindowFlag::Decorated | LWGC::WindowFlag::Focused);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}
