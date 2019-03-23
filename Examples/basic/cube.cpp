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
	app.Open("Test Window", 2560, 1440, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	auto textureMaterial = Material::Create("Shaders/Debug/TextureAtlas.hlsl");

	auto atlas = Texture2DAtlas::Create(2048, 2048, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);
	
	Rect pos1 = atlas->Fit("Images/512_1.png");
	Rect pos2 = atlas->Fit("Images/512_2.png");

	atlas->UploadAtlasDatas();

	auto animeTexture = Texture2D::Create("Images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());

	// ImGUI test
	cube->AddComponent(new ImGUIPanel());

	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);

	textureMaterial->SetTexture(TextureBinding::Albedo, atlas, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
	textureMaterial->SetBuffer("sizeOffsets", atlas->GetSizeOffsetBuffer(), atlas->GetSizeOffsetBufferSize(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	// textureMaterial->SetBuffer("atlasSize", atlas->GetAtlasSizeBuffer(), atlas->GetAtlasSizeBufferSize(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

	ProcessEvent(es, app);
	atlas->Clear();
	while (app.ShouldNotQuit())
		app.Update();
	return (0);
}