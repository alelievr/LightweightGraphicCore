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
	
	// Temporary stuff, must be handled by the material and the render pass
	static float test[10];
	VkBuffer buffer;
	VkDeviceMemory mem;
	{
		auto binding1 = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_ALL);
		// auto binding2 = Vk::CreateDescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL);
		VkDescriptorSetLayout layout;
		Vk::CreateDescriptorSetLayout({binding1}, layout);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = VulkanInstance::Get()->GetDescriptorPool();
		allocInfo.descriptorSetCount = 1u;
		allocInfo.pSetLayouts = &layout;
		Vk::CreateBuffer(sizeof(test), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, mem);

		auto device = VulkanInstance::Get()->GetDevice();

		VkDescriptorSet set;
		if (vkAllocateDescriptorSets(device, &allocInfo, &set) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");

		// writeProceduralTexture->SetDescriptorSet(set);
	}
	// End Temporary stuff

	Texture2D jibril("images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, true);

	// auto testMat = std::make_shared< Material >(BuiltinShaders::Standard);
	// auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, testMat));
	auto cube = new GameObject(new MeshRenderer(PrimitiveType::Cube, displayProceduralTexture));
	auto cam = new GameObject(new Camera());
	const int workGroupSize = 8; // Work group size in the compute shader
	// cube->AddComponent(new ComputeDispatcher(writeProceduralTexture, 512 / workGroupSize, 512 / workGroupSize, &jibril));
	cube->GetTransform()->Translate(glm::vec3(0, 1, 0));
	cam->AddComponent(new FreeCameraControls());
	hierarchy->AddGameObject(cube);
	hierarchy->AddGameObject(cam);
	// testMat->SetTexture(TextureBinding::Albedo, jibril, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	// writeProceduralTexture->SetTexture("proceduralTexture", jibril, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
	// writeProceduralTexture->SetBuffer("f", buffer, sizeof(test), VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
	displayProceduralTexture->SetTexture(TextureBinding::Albedo, jibril, VK_IMAGE_LAYOUT_GENERAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

	while (app.ShouldNotQuit())
	{
		app.Update();
		ProcessEvent(es, app);
	}
	return (0);
}