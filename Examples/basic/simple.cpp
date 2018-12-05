#include "LWGC.hpp"

using LWGC;

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

int			main(void)
{
	Application		app;
	EventSystem *	es = app.GetEventSystem();
	Hierarchy *		hierarchy = app.GetHierarchy();

	ShaderSource::AddIncludePath("../../");

	//Initialize application
	app.Init();

	Texture2D	proceduralTexture(512, 512, VK_FORMAT_R8G8B8_UNORM, VK_USAGE, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	Material	writeProceduralTexture("Shaders/Compute/ProceduralTexture.hlsl");
	Material	displayProceduralTexture(ShaderProgram::Standard); // Copy of the standard material

	displayProceduralTexture.SetTexture(TextureBinding::Albedo, proceduralTexture);
	// We manually set the binding index because it's a custom compute shader and we know it's layout
	writeProceduralTexture.SetTexture(0, proceduralTexture);

	auto testMat = std::make_shared< Material >(ShaderProgram::Standard);
	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, testMat));
	auto cam = new GameObject(new Camera());
	cube->GetTransform()->Translate(glm::vec3(0, 1, 0));
	cam->AddComponent(new FreeCameraControls());
	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);

	hierarchy->AddGameObject(new ComputeDispatcher());

	//open window
	app.Open("Test Window", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}
