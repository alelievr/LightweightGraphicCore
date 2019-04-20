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

Material *	CreateTransparentMaterial(void)
{
	auto mat = Material::Create("Shaders/Debug/TextureAtlas.hlsl");

	mat->SetColorBlendState(MaterialState::defaultColorBlendState);
	mat->SetDepthStencilState(MaterialState::depthCompareWrite);

	return mat;
}

Material *	CreateBackgroundMaterial(void)
{
	auto		fullScreenMaterial = Material::Create("Shaders/Debug/AlbedoTexture.hlsl", BuiltinShaders::FullScreenQuad);

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
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_GREATER;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = VK_FALSE;

	fullScreenMaterial->SetDepthStencilState(depthStencilInfo);

	return fullScreenMaterial;
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
	app.Open("Test Window", 1920, 1200, WindowFlag::Resizable | WindowFlag::Decorated | WindowFlag::Focused);

	auto textureMaterial = CreateTransparentMaterial(); //Material::Create("Shaders/Debug/TextureAtlas.hlsl");

	auto atlas = Texture2DAtlas::Create(2048, 2048, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	Rect pos1 = atlas->Fit("Images/512_1.png");
	Rect pos2 = atlas->Fit("Images/512_2.png");

	atlas->UploadAtlasDatas();

	auto animeTexture = Texture2D::Create("Images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto backgroundMaterial = CreateBackgroundMaterial();

	auto cube1 = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cube2 = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());
	cam->AddComponent(new ProceduralRenderer(backgroundMaterial, 4));

	// ImGUI test
	cube1->AddComponent(new ProfilerPanel());

	cube1->GetTransform()->Translate(glm::vec3(-1, 0, 0));
	cube2->GetTransform()->Translate(glm::vec3(1, 0, 0));

	hierarchy->AddGameObject(cube1);
	hierarchy->AddGameObject(cube2);
	hierarchy->AddGameObject(cam);

	// auto model = ModelLoader::Load(@"Models/Heavy_FullMetal_Sword/Heavy Full Metal Sword.fbx");
	// hierarchy->AddGameObject(model);

	backgroundMaterial->SetTexture(TextureBinding::Albedo, animeTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	textureMaterial->SetTexture(TextureBinding::Albedo, atlas, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
	textureMaterial->SetBuffer("sizeOffsets", atlas->GetSizeOffsetBuffer(), atlas->GetSizeOffsetBufferSize(), VK_DESCRIPTOR_TYPE_STORAGE_BUFFER); // StructuredBuffer are always storage buffers even when not RW
	// Not supported on Metal 1 (lol)
	// textureMaterial->SetTexelBuffer("atlasSize", atlas->GetAtlasSizeBuffer(), atlas->GetAtlasSizeBufferSize(), VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER);

	ProcessEvent(es, app);
	atlas->Clear();
	while (app.ShouldNotQuit())
		app.Update();
	return (0);
}