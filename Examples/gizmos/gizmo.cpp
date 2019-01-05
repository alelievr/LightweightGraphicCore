#include "LWGC.hpp"

using namespace LWGC;

void		ProcessEvent(EventSystem * es, Application & app)
{
	const Event &	current = es->GetCurrentEvent();

	auto keyCode = current.GetKeyCode();

	switch (current.GetType())
	{
		case EventType::KeyDown:
			if (keyCode == KeyCode::ESCAPE)
				app.Quit();
			break ;

		default:
			break ;
	}
}

void		InitGizmos(Hierarchy * hierarchy)
{
	auto lineGizmo = new Gizmo::Line(glm::vec3(-1, 0, 0), glm::vec3(0, 4, 0), Color::Yellow);
	lineGizmo->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(lineGizmo);

	auto rayGizmo = new Gizmo::Ray(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), Color::Red);
	rayGizmo->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(rayGizmo);

	auto frustum = new Gizmo::Frustum(Math::DegToRad * 30, 16 / 9.0, 0.1, 10, Color::Blue);
	frustum->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(frustum);
}

void		InitCamera(Hierarchy * hierarchy)
{
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());
	hierarchy->AddGameObject(cam);
}

int			main(void)
{
	Application		app;
	EventSystem *	es = app.GetEventSystem();
	Hierarchy *		hierarchy = app.GetHierarchy();

	ShaderSource::AddIncludePath("../../");

	//Initialize application
	app.Init();

	// We must Open the window before doing anything related to vulkan
	app.Open("Test Window", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	InitGizmos(hierarchy);
	InitCamera(hierarchy);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}