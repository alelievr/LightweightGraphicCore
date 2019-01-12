#include "LWGC.hpp"

using namespace LWGC;

static Gizmo::Ray * r;
static int i;

void	ProcessEvent(EventSystem * es, Application & app)
{
	es->Get()->onKey.AddListener([&](KeyCode key, ButtonAction action)
		{
			if (action == ButtonAction::Press
				&& key == KeyCode::ESCAPE)
					app.Quit();
		}
	);
}

void		InitGizmos(Hierarchy * hierarchy)
{
	auto lineGizmo = new Gizmo::Line(glm::vec3(-1, 0, 0), glm::vec3(0, 4, 0), Color::Yellow);
	lineGizmo->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(lineGizmo);

	auto rayGizmo = r = new Gizmo::Ray(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), Color::Red);
	rayGizmo->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(rayGizmo);

	auto frustum = new Gizmo::Frustum(Math::DegToRad * 30, 16 / 9.0, 0.1, 10, true, Color::White);
	frustum->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	hierarchy->AddGameObject(frustum);

	auto circle = new Gizmo::Circle(2, 40, true, Color::Indigo);
	circle->GetTransform()->SetPosition(glm::vec3(0, -2, 0));
	hierarchy->AddGameObject(circle);

	auto cone = new Gizmo::Cone(2, 1, 40, true, Color::DarkGreen);
	cone->GetTransform()->SetPosition(glm::vec3(-2, 0, 1));
	hierarchy->AddGameObject(cone);

	auto arrow = new Gizmo::Arrow(2, 50, false, Color::LightGray);
	arrow->GetTransform()->SetPosition(glm::vec3(2, 0, -1));
	arrow->AddComponent(new Rotator());
	hierarchy->AddGameObject(arrow);

	auto position = new Gizmo::Position(glm::vec3(0, 0, -5));
	hierarchy->AddGameObject(position);

	// selection and handle test for cube:
	hierarchy->AddGameObject(new GameObject(new MeshRenderer(PrimitiveType::Cube)));
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