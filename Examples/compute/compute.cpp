#include "LWGC.hpp"

using namespace LWGC;

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
void		InitFullscreenTarget(Hierarchy * hierarchy)
{
	auto		fullScreenMaterial = Material::Create(BuiltinShaders::Standard, BuiltinShaders::FullScreenQuad);
	Texture2D	computeTarget(512, 512, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, false);

	// Setup material to display the procedural texture:
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	fullScreenMaterial->SetInputAssemblyState(inputAssemblyInfo);

	// VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
	// depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	// depthStencilInfo.depthTestEnable = VK_TRUE;
	// depthStencilInfo.depthWriteEnable = VK_FALSE;
	// depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	// depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	// depthStencilInfo.stencilTestEnable = VK_FALSE;

	// fullScreenMaterial->SetDepthStencilState(depthStencilInfo);

	hierarchy->AddGameObject(new GameObject(new ProceduralRenderer(fullScreenMaterial, 4)));
	fullScreenMaterial->SetTexture(TextureBinding::Albedo, computeTarget, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	auto	proceduralDispatchMaterial = Material::Create("Shaders/Compute/ProceduralTexture.hlsl", VK_SHADER_STAGE_COMPUTE_BIT);
	hierarchy->AddGameObject(new GameObject(new ComputeDispatcher(proceduralDispatchMaterial, 512, 512)));
	proceduralDispatchMaterial->SetTexture("proceduralTexture", computeTarget, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
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
	app.Open("Compute Test", 1920, 1080, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	InitCamera(hierarchy);
	InitFullscreenTarget(hierarchy);

		ProcessEvent(es, app);
	while (app.ShouldNotQuit())
		app.Update();

	return (0);
}