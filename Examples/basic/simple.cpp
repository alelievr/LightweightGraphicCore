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

	// We must Open the window before doing anything related to vulkan
	app.Open("Test Window", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	Texture2D	proceduralTexture(512, 512, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	// auto	writeProceduralTexture = std::make_shared< Material >("Shaders/Compute/ProceduralTexture.hlsl", VK_SHADER_STAGE_COMPUTE_BIT);
	auto	displayProceduralTexture = std::make_shared< Material >(BuiltinShaders::Standard); // Copy of the standard material
	auto	fullScreenTest = std::make_shared< Material >(BuiltinShaders::Standard, "Shaders/FullScreenQuad.hlsl");

	auto	writeTexture = std::make_shared< Material >("Shaders/Debug/TextureWrite.hlsl", "Shaders/FullScreenQuad.hlsl");

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	fullScreenTest->SetInputAssemblyState(inputAssemblyInfo);

	writeTexture->SetInputAssemblyState(inputAssemblyInfo);

	Texture2D jibril("images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, true);

	// auto testMat = std::make_shared< Material >(BuiltinShaders::Standard);
	// auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, testMat));
	auto cube1 = new GameObject(new MeshRenderer(PrimitiveType::Cube, displayProceduralTexture));
	auto fullScreen = new GameObject(new ProceduralRenderer(fullScreenTest, 4));
	auto writeTextureObject = new GameObject(new ProceduralRenderer(writeTexture, 4));
	auto cam = new GameObject(new Camera());
	const int workGroupSize = 8; // Work group size in the compute shader
	// cube1->AddComponent(new ComputeDispatcher(writeProceduralTexture, 512 / workGroupSize, 512 / workGroupSize, &jibril));
	cube1->GetTransform()->Translate(glm::vec3(0, 1, 0));

	// srand(time(NULL) + clock());
	// for (int i = 0; i < 100; i++)
	// {
	// 	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, displayProceduralTexture));
	// 	cube->GetTransform()->Translate(glm::vec3(rand() % 100, rand() % 100, rand() % 100));
	// 	hierarchy->AddGameObject(cube);
	// }

	cam->AddComponent(new FreeCameraControls());
	hierarchy->AddGameObject(cube1);
	hierarchy->AddGameObject(fullScreen);
	hierarchy->AddGameObject(writeTextureObject);
	hierarchy->AddGameObject(cam);
	// testMat->SetTexture(TextureBinding::Albedo, jibril, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	// Reserve memory so we don't have to allocate and bind the descriptor set
	// writeProceduralTexture->AllocateDescriptorSet("proceduralTexture");
	// writeProceduralTexture->SetTexture("proceduralTexture", jibril, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
	// "f" is inside the same descriptor set than procedural texture so we don't need to allocate his descriptor set
	// writeProceduralTexture->SetBuffer("f", buffer, sizeof(test), VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

	writeTexture->AllocateDescriptorSet("writeTexture");
	writeTexture->SetTexture("writeTexture", proceduralTexture, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
	displayProceduralTexture->SetTexture(TextureBinding::Albedo, proceduralTexture, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}