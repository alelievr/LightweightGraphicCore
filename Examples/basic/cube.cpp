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

	static VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	// colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	// colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

	// Additive:
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;

	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	static VkPipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.logicOpEnable = VK_FALSE;
	colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorBlendAttachment;
	colorBlendState.blendConstants[0] = 0.0f;
	colorBlendState.blendConstants[1] = 0.0f;
	colorBlendState.blendConstants[2] = 0.0f;
	colorBlendState.blendConstants[3] = 0.0f;

	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthTestEnable = VK_TRUE;
	depthStencilState.depthWriteEnable = VK_FALSE;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilState.depthBoundsTestEnable = VK_FALSE;
	depthStencilState.minDepthBounds = 0.0f; // Optional
	depthStencilState.maxDepthBounds = 1.0f; // Optional
	depthStencilState.stencilTestEnable = VK_FALSE;
	depthStencilState.front = {}; // Optional
	depthStencilState.back = {}; // Optional

	mat->SetColorBlendState(colorBlendState);
	mat->SetDepthStencilState(depthStencilState);

	return mat;
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

	auto textureMaterial = CreateTransparentMaterial(); //Material::Create("Shaders/Debug/TextureAtlas.hlsl");

	auto atlas = Texture2DAtlas::Create(2048, 2048, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	Rect pos1 = atlas->Fit("Images/512_1.png");
	Rect pos2 = atlas->Fit("Images/512_2.png");

	atlas->UploadAtlasDatas();

	auto animeTexture = Texture2D::Create("Images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true);

	auto cube1 = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cube2 = new GameObject(new MeshRenderer(PrimitiveType::Cube, textureMaterial));
	auto cam = new GameObject(new Camera());
	cam->GetTransform()->SetPosition(glm::vec3(0, 0, -5));
	cam->AddComponent(new FreeCameraControls());

	// ImGUI test
	cube1->AddComponent(new ImGUIPanel());

	cube1->GetTransform()->Translate(glm::vec3(-1, 0, 0));
	cube2->GetTransform()->Translate(glm::vec3(1, 0, 0));

	hierarchy->AddGameObject(cube1);
	hierarchy->AddGameObject(cube2);
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