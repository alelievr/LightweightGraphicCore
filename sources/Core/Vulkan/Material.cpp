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

	if (descriptorSetLayout == VK_NULL_HANDLE)
		CreateDescriptorSetLayout();
	
	CreateGraphicPipeline();
	CreateTextureImage();
	CreateTextureSampler();
	CreateUniformBuffer();
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

	auto perMaterialBinding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
	auto albedoBinding = Vk::CreateDescriptorSetLayoutBinding(ALBEDO_BINDING_INDEX, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_ALL_GRAPHICS);
	auto samplerBinding = Vk::CreateDescriptorSetLayoutBinding(TRILINEAR_CLAMP_BINDING_INDEX, VK_DESCRIPTOR_TYPE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);

	Vk::CreateDescriptorSetLayout({perMaterialBinding, albedoBinding, samplerBinding}, descriptorSetLayout);
}

void					Material::CreateGraphicPipeline(void)
{
	_program.SetFragmentSourceFile("shaders/debug/AlbedoTextue.hlsl");
	_program.SetVertexSourceFile("shaders/DefaultVertex.hlsl");

	_program.CompileAndLink();

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
	pipelineInfo.pStages = _program.GetShaderStages();
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

	Vk::currentPipelineLayout = _graphicPipelineLayout;

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
	_samplers.push_back(Vk::Samplers::trilinearClamp);
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

void					Material::UpdateUniformBuffer()
{
	_perMaterial.albedo = glm::vec4(1, 1, 0, 1);

	void* data;
	vkMapMemory(_device, _uniformPerMaterial.memory, 0, sizeof(_perMaterial), 0, &data);
	memcpy(data, &_perMaterial, sizeof(_perMaterial));
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
	imageInfo.sampler = 0;

    VkDescriptorImageInfo samplerInfo = {};
    samplerInfo.sampler = Vk::Samplers::trilinearClamp;

	std::array<VkWriteDescriptorSet, 3> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _descriptorSet;
	descriptorWrites[1].dstBinding = ALBEDO_BINDING_INDEX;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstSet = _descriptorSet;
	descriptorWrites[2].dstBinding = TRILINEAR_CLAMP_BINDING_INDEX;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pImageInfo = &samplerInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void					Material::BindDescriptorSets(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint)
{
	vkCmdBindDescriptorSets(cmd, bindPoint, _graphicPipelineLayout, PER_MATERIAL_BINDING_INDEX, 1, &_descriptorSet, 0, nullptr);
}

// TODO: move this elsewhere

VkPipelineLayout		Material::GetGraphicPipelineLayout(void) const { return (this->_graphicPipelineLayout); }
void					Material::SetGraphicPipelineLayout(VkPipelineLayout tmp) { this->_graphicPipelineLayout = tmp; }

VkPipeline				Material::GetGraphicPipeline(void) const { return (this->_graphicPipeline); }
void					Material::SetGraphicPipeline(VkPipeline tmp) { this->_graphicPipeline = tmp; }

VkDescriptorSetLayout	Material::GetDescriptorSetLayout(void)
{
	if (descriptorSetLayout == VK_NULL_HANDLE)
		CreateDescriptorSetLayout();
	
	return descriptorSetLayout;
}

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "Material" << std::endl;
	(void)r;
	return (o);
}
