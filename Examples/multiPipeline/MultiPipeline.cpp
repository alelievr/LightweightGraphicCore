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

class FirstPipeline : public RenderPipeline
{
	public:
		void	Render(const std::vector< Camera * > & cameras, RenderContext * context) override
		{
			RenderPipeline::Render(cameras, context);
		}
};

class SecondPipeline : public RenderPipeline
{
	public:
		void	Render(const std::vector< Camera * > & cameras, RenderContext * context) override
		{
			RenderPipeline::Render(cameras, context);
		}
};

void SwitchPipeline(RenderPipeline * p1, RenderPipeline * p2)
{
	static int pipe = 0;

	if (pipe % 2)
	{
		RenderPipelineManager::SetCurrentRenderPipeline(p1);
	}
	else
	{
		RenderPipelineManager::SetCurrentRenderPipeline(p2);
	}

	pipe++;
}

int			main(void)
{
	Application		app(false);
	EventSystem *	es = app.GetEventSystem();
	Hierarchy *		hierarchy = app.GetHierarchy();

	auto p1 = new FirstPipeline();
	auto p2 = new SecondPipeline();

	ShaderSource::AddIncludePath("../../");

	//Initialize application
	app.Init();

	// We must Open the window before doing anything related to vulkan
	app.Open("Test Window", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	auto textureMaterial = Material::Create(BuiltinShaders::Standard);
	auto animeTexture = Texture2D::Create("Images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());

	// ImGUI test
	cube->AddComponent(new ImGUIPanel());

	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);

	textureMaterial->SetTexture(TextureBinding::Albedo, animeTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	ProcessEvent(es, app);
	while (app.ShouldNotQuit())
	{
		if ((Time::GetFrameCount() % 60) == 0)
			SwitchPipeline(p1, p2);
		app.Update();
	}
	return (0);
}