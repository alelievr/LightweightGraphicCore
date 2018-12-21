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
	auto	writeProceduralTexture = std::make_shared< Material >("Shaders/Compute/ProceduralTexture.hlsl", VK_SHADER_STAGE_COMPUTE_BIT);
	auto	fullScreenTest = std::make_shared< Material >(BuiltinShaders::Standard, BuiltinShaders::FullScreenQuad);
	auto	anime = std::make_shared< Material >(BuiltinShaders::Standard);

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	fullScreenTest->SetInputAssemblyState(inputAssemblyInfo);

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.depthTestEnable = VK_TRUE;
	depthStencilInfo.depthWriteEnable = VK_FALSE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = VK_FALSE;

	fullScreenTest->SetDepthStencilState(depthStencilInfo);

	Texture2D jibril("images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, true);
	Texture2D animeTexture("images/567634.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);
	Texture2D possiblyYelloTexture(512, 512, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, false);

	auto cube1 = new GameObject(new MeshRenderer(PrimitiveType::Cube, anime));
	auto fullScreen = new GameObject(new ProceduralRenderer(fullScreenTest, 4));
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cube1->AddComponent(new ComputeDispatcher(writeProceduralTexture, 512, 512));
	cube1->GetTransform()->Scale(glm::vec3(.2, .2, 1));
	// cube1->AddComponent(new FreeCameraControls());

	srand(time(NULL) + clock());
	for (int i = 0; i < 100; i++)
	{
		auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, anime));
		cube->GetTransform()->Translate(glm::vec3(rand() % 100, rand() % 100, rand() % 100));
		hierarchy->AddGameObject(cube);
	}

	cam->AddComponent(new FreeCameraControls());
	hierarchy->AddGameObject(cube1);
	hierarchy->AddGameObject(fullScreen);
	hierarchy->AddGameObject(cam);

	// Reserve memory so we don't have to allocate and bind the descriptor set
	writeProceduralTexture->AllocateDescriptorSet("proceduralTexture");
	writeProceduralTexture->SetTexture("proceduralTexture", possiblyYelloTexture, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
	anime->SetTexture(TextureBinding::Albedo, animeTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
	fullScreenTest->SetTexture(TextureBinding::Albedo, possiblyYelloTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}