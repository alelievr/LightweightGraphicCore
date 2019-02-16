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

Material *		CreateFullscreenMaterial(void)
{
	auto		fullScreenMaterial = Material::Create(BuiltinShaders::Standard, BuiltinShaders::FullScreenQuad);

	// Setup material to display the procedural texture:
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	fullScreenMaterial->SetInputAssemblyState(inputAssemblyInfo);

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.depthTestEnable = VK_TRUE;
	depthStencilInfo.depthWriteEnable = VK_FALSE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = VK_FALSE;

	fullScreenMaterial->SetDepthStencilState(depthStencilInfo);

	return fullScreenMaterial;
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

	auto	writeProceduralTexture = Material::Create("Shaders/Compute/ProceduralTexture.hlsl", VK_SHADER_STAGE_COMPUTE_BIT);
	auto	displayProceduralTexture = CreateFullscreenMaterial();

	auto proceduralTexture = Texture2D::Create(512, 512, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, false);

	InitCamera(hierarchy);

	auto computeProcessor = new GameObject(new ComputeDispatcher(writeProceduralTexture, 512, 512));
	hierarchy->AddGameObject(computeProcessor);

	// fullscreen compute display
	hierarchy->AddGameObject(new GameObject(new ProceduralRenderer(displayProceduralTexture, 4)));

	// Reserve memory so we don't have to allocate and bind the descriptor set
	writeProceduralTexture->SetTexture("proceduralTexture", proceduralTexture, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
	displayProceduralTexture->SetTexture(TextureBinding::Albedo, proceduralTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	ProcessEvent(es, app);
	while (app.ShouldNotQuit())
		app.Update();

	return (0);
}