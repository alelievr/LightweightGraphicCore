#include "Material.hpp"

#include <fstream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include GLM_INCLUDE
#include GLM_INCLUDE_MATRIX_TRANSFORM

#include "Core/Texture2DArray.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Mesh.hpp"

using namespace LWGC;

VkDescriptorSetLayout Material::descriptorSetLayout;

Material::Material(void)
{
	this->_graphicPipelineLayout = VK_NULL_HANDLE;
	this->_graphicPipeline = VK_NULL_HANDLE;
}

Material::Material(Material const & src)
{
	*this = src;
}

Material::~Material(void)
{
	vkDeviceWaitIdle(_instance->GetDevice());

	CleanupGraphicPipeline();

	delete _textures[0];

	vkDestroySampler(_device, _samplers[0], nullptr);

	// Warning: destroying any material will destroy the layout for every other materials !
	vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);

	vkDestroyBuffer(_device, _uniformPerMaterial.buffer, nullptr);
	vkFreeMemory(_device, _uniformPerMaterial.memory, nullptr);
}

Material &	Material::operator=(Material const & src)
{
	if (this != &src)
	{
		this->_graphicPipelineLayout = src._graphicPipelineLayout;
		this->_graphicPipeline = src._graphicPipeline;
		this->_uniformPerMaterial = src._uniformPerMaterial;
		this->_samplers = src._samplers;
		this->_textures = src._textures;
		this->_instance = src._instance;
		this->_device = src._device;
		this->_swapChain = src._swapChain;
	}
	return (*this);
}

void					Material::Initialize(SwapChain * swapChain, RenderPass * renderPass)
{
	_instance = VulkanInstance::Get();
	_device = _instance->GetDevice();
	_swapChain = swapChain;
	_renderPass = renderPass;

	CreateDescriptorSetLayout();
	CreateGraphicPipeline();
	CreateTextureImage();
	CreateTextureSampler();
	CreateUniformBuffer();
	CreateDescriptorPool();
	CreateDescriptorSets();
}

void					Material::CleanupGraphicPipeline(void) noexcept
{
	vkDestroyPipeline(_device, _graphicPipeline, nullptr);
	vkDestroyPipelineLayout(_device, _graphicPipelineLayout, nullptr);
}

void	Material::CreateDescriptorSetLayout(void)
{
	if (descriptorSetLayout != VK_NULL_HANDLE)
		return ;
	
	auto perMaterialBinding = Vk::CreateDescriptorSetLayoutBinding(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
	auto albedoBinding = Vk::CreateDescriptorSetLayoutBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);

	Vk::CreateDescriptorSetLayout({perMaterialBinding, albedoBinding}, descriptorSetLayout);
}

// TODO: same here
static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
	    throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule	Material::createShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	return shaderModule;
}

void					Material::CreateGraphicPipeline(void)
{
	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Mesh::GetBindingDescription();
	auto attributeDescriptions = Mesh::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	const auto & extent = _swapChain->GetExtent();
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) extent.width;
	viewport.height = (float) extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	const auto & setLayouts = VulkanRenderPipeline::GetUniformSetLayouts();
	pipelineLayoutInfo.setLayoutCount = setLayouts.size();
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();

	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_graphicPipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _graphicPipelineLayout;
	pipelineInfo.renderPass = _renderPass->GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &depthStencil;

	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");

	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
	vkDestroyShaderModule(_device, vertShaderModule, nullptr);

	printf("Graphic pipeline created !\n");
}

#include "Core/Texture2D.hpp"

void					Material::CreateTextureImage(void)
{
	_textures.reserve(1);

	_textures.push_back((Texture *)(new Texture2D("images/656218.jpg", VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT, true)));

	// Texture arrays are currently not supported and require MSL 2.0 :(
	// _textures.push_back((Texture *)(new Texture2D(2, 2, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT)));
	// auto textureArray = new Texture2DArray(512, 512, 2, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT);

	// try {
	// 	textureArray->SetImage("images/512_1.png", 0);
	// 	textureArray->SetImage("images/512_2.png", 1);
	// } catch (std::runtime_error r) {
	// 	std::cout << r.what() << std::endl;
	// }

	// textureArray->Upload();

	// _textures.push_back((Texture *)(textureArray));
}

void					Material::CreateTextureSampler(void)
{
	_samplers.push_back(Vk::Samplers::trilinearRepeat);
}

void					Material::CreateUniformBuffer(void)
{
	Vk::CreateBuffer(
		sizeof(LWGC_PerMaterial),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformPerMaterial.buffer,
		_uniformPerMaterial.memory
	);
}

void					Material::CreateDescriptorPool(void)
{
}

void					Material::UpdateUniformBuffer()
{
	// static auto startTime = std::chrono::high_resolution_clock::now();

	// auto currentTime = std::chrono::high_resolution_clock::now();
	// float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	LWGC_PerMaterial perMaterial = {};
	perMaterial.albedo = glm::vec4(1, 1, 0, 1);
	
	// TODO: move this to camera and meshRenderer buffers
	// perMaterial.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// perMaterial.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// perMaterial.proj = glm::perspective(glm::radians(45.0f), _swapChain->GetExtent().width / (float) _swapChain->GetExtent().height, 0.1f, 10.0f);
	// // GLM projection matrix was designed for OpenGL where y is flipped unlike every other graphic API
	// perMaterial.proj[1][1] *= -1;

	void* data;
	vkMapMemory(_device, _uniformPerMaterial.memory, 0, sizeof(perMaterial), 0, &data);
	memcpy(data, &perMaterial, sizeof(perMaterial));
	vkUnmapMemory(_device, _uniformPerMaterial.memory);
}

void					Material::CreateDescriptorSets(void)
{
	std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _instance->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(_device, &allocInfo, &_descriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformPerMaterial.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerMaterial);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = _textures[0]->GetView();
	imageInfo.sampler = _samplers[0];

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _descriptorSet;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void					Material::BindDescriptorSets(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint)
{
	vkCmdBindDescriptorSets(cmd, bindPoint, _graphicPipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);
}

// TODO: move this elsewhere

VkPipelineLayout		Material::GetGraphicPipelineLayout(void) const { return (this->_graphicPipelineLayout); }
void					Material::SetGraphicPipelineLayout(VkPipelineLayout tmp) { this->_graphicPipelineLayout = tmp; }

VkPipeline				Material::GetGraphicPipeline(void) const { return (this->_graphicPipeline); }
void					Material::SetGraphicPipeline(VkPipeline tmp) { this->_graphicPipeline = tmp; }

VkDescriptorSetLayout	Material::GetDescriptorSetLayout(void)
{
	return descriptorSetLayout;
}

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "Material" << std::endl;
	(void)r;
	return (o);
}
