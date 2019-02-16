#include "LWGC.hpp"

using namespace LWGC;

void	ProcessEvent(EventSystem * es, Application & app)
{
	es->Get()->onKey.AddListener([&](KeyCode key, ButtonAction action, int)
		{
			if (action == ButtonAction::Press
				&& key == KeyCode::ESCAPE)
					app.Quit();
		}
	);
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

	auto	textureMaterial = Material::Create(BuiltinShaders::Standard);

	auto animeTexture = Texture2D::Create("Images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cube2 = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));

	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());

	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cube2);
	hierarchy->AddGameObject(cam);

	textureMaterial->SetTexture(TextureBinding::Albedo, animeTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	ProcessEvent(es, app);

	float degree = 10.0;
	float scale = 0.5;
	auto tmptime = Time::GetTime();
	auto tmptime2 = Time::GetUnscaledTime();
	auto tmpUnscaledtime = Time::GetUnscaledTime();

	Time::SetScale(scale);
	cube->GetTransform()->SetPosition(glm::vec3(3, 0, 0));

	while (app.ShouldNotQuit())
	{
		app.Update();

		if ((Time::GetTime() - tmptime) > 0.5)
		{
			tmptime = Time::GetTime();
			cube->GetTransform()->RotateAxis(degree * Math::DegToRad, glm::vec3(0, 1, 0));
		}

		if ((Time::GetUnscaledTime() - tmptime2) > 0.5)
		{
			tmptime2 = Time::GetUnscaledTime();
			cube2->GetTransform()->RotateAxis(degree * Math::DegToRad, glm::vec3(0, 1, 0));
		}

		if ((Time::GetUnscaledTime() - tmpUnscaledtime) > 10.0)
		{
			std::cout << " ====== Scale => " << Time::GetTimeScale() << std::endl;
			scale =  scale < 2.0 ? scale + 0.5 : 0.5;
			Time::SetScale(scale);
			tmpUnscaledtime = Time::GetUnscaledTime();
			std::cout.precision(17);

			std::cout << "Time => " << Time::GetTime() << std::endl;
			std::cout << "UnscaledTime => " << Time::GetUnscaledTime() << std::endl;

			std::cout << "deltaTime => " << Time::GetDeltaTime() << std::endl;
			std::cout << "GetUnscaledDeltaTime => " << Time::GetUnscaledDeltaTime() << std::endl;
		}
	}
	return (0);
}
