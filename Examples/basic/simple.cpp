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

int			main(void)
{
	Application		app;
	EventSystem *	es = app.GetEventSystem();
	Hierarchy *		hierarchy = app.GetHierarchy();

	ShaderSource::AddIncludePath("../../");

	//Initialize application
	app.Init();

	//open window
	app.Open("Test Window", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	// Texture2D	proceduralTexture(512, 512, VK_FORMAT_R8G8B8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	// Material	writeProceduralTexture("Shaders/Compute/ProceduralTexture.hlsl", VK_SHADER_STAGE_COMPUTE_BIT);
	// Material	displayProceduralTexture(BuiltinShaders::Standard); // Copy of the standard material

	// VkDescriptorSetLayout proceduralTextureLayout;
	// Vk::CreateDescriptorSetLayout({Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT)}, proceduralTextureLayout);

	// set 0: frame datas and set 1: RWTexture
	// writeProceduralTexture.SetDescriptorSetLayout(0, VulkanRenderPipeline::GetGraphicUniformSetLayouts());
	// writeProceduralTexture.SetDescriptorSetLayout(1, proceduralTextureLayout);

	// displayProceduralTexture.SetTexture(TextureBinding::Albedo, proceduralTexture);
	// We manually set the binding index because it's a custom compute shader and we know it's layout
	// writeProceduralTexture.SetTexture(1, 0, proceduralTexture);

	Texture2D jibril("images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto testMat = std::make_shared< Material >(BuiltinShaders::Standard);
	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, testMat));
	auto cam = new GameObject(new Camera());
	cube->GetTransform()->Translate(glm::vec3(0, 1, 0));
	cam->AddComponent(new FreeCameraControls());
	// hierarchy->AddGameObject(new ComputeDispatcher(writeProceduralTexture, 512, 512));
	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);
	testMat->SetTexture(TextureBinding::Albedo, jibril, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}
