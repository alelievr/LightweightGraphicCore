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
#include "Core/Shaders/BuiltinShaders.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Vulkan/RenderPass.hpp"

using namespace LWGC;

const std::string	TextureBinding::Albedo = "albedoMap";
const std::string	TextureBinding::Normal = "normalMap";
const std::string	TextureBinding::Height = "heightMap";
const std::string	TextureBinding::Smoothness = "smoothnessMap";

const std::string	SamplerBinding::TrilinearClamp = "trilinearClamp";
const std::string	SamplerBinding::TrilinearRepeat = "trilinearRepeat";
const std::string	SamplerBinding::NearestClamp = "nearestClamp";
const std::string	SamplerBinding::NearestRepeat = "nearestRepeat";
const std::string	SamplerBinding::AnisotropicTrilinearClamp = "anisotropicTrilinearClamp";
const std::string	SamplerBinding::DepthCompare = "depthCompare";

Material::Material(void)
{
	this->_pipelineLayout = VK_NULL_HANDLE;
	this->_pipeline = VK_NULL_HANDLE;
	_program = new ShaderProgram();
	_program->SetSourceFile("Shaders/Error/Pink.hlsl", VK_SHADER_STAGE_FRAGMENT_BIT);
	_program->SetSourceFile(BuiltinShaders::DefaultVertex, VK_SHADER_STAGE_VERTEX_BIT);
}

Material::Material(const std::string & shader, VkShaderStageFlagBits stage)
{
	_program = new ShaderProgram();
	_program->SetSourceFile(shader, stage);
}

Material::Material(const std::string & fragmentShader)
{
	_program = new ShaderProgram();
	_program->SetSourceFile(fragmentShader, VK_SHADER_STAGE_FRAGMENT_BIT);
	_program->SetSourceFile(BuiltinShaders::DefaultVertex, VK_SHADER_STAGE_VERTEX_BIT);
}

Material::Material(ShaderProgram * program)
{
	_program = program;
}

Material::Material(Material const & src)
{
	*this = src;
}

Material::~Material(void)
{
	delete _program;

	// Don't delete if the material have not been initialized
	if (_instance == nullptr)
		return ;
	
	vkDeviceWaitIdle(_instance->GetDevice());

	CleanupPipeline();

	vkDestroySampler(_device, _samplers[0], nullptr);

	vkDestroyBuffer(_device, _uniformPerMaterial.buffer, nullptr);
	vkFreeMemory(_device, _uniformPerMaterial.memory, nullptr);
}

Material &	Material::operator=(Material const & src)
{
	if (this != &src)
	{
		this->_pipelineLayout = src._pipelineLayout;
		this->_pipeline = src._pipeline;
		this->_uniformPerMaterial = src._uniformPerMaterial;
		this->_samplers = src._samplers;
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

	CompileShaders();
	CreatePipelineLayout();
	CreatePipeline();
	CreateTextureSampler();
	CreateUniformBuffer();
	CreateDescriptorSets();
}

void					Material::CleanupPipeline(void) noexcept
{
	vkDestroyPipeline(_device, _pipeline, nullptr);
	vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
}

// void	Material::CreateGraphicDescriptorSetLayout(void)
// {
// 	auto perMaterialBinding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
// 	auto albedoBinding = Vk::CreateDescriptorSetLayoutBinding(TextureBinding::Albedo, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_ALL_GRAPHICS);
// 	auto samplerBinding = Vk::CreateDescriptorSetLayoutBinding(TRILINEAR_CLAMP_BINDING_INDEX, VK_DESCRIPTOR_TYPE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);

// 	Vk::CreateDescriptorSetLayout({perMaterialBinding, albedoBinding, samplerBinding}, graphicDescriptorSetLayout);
// }

void					Material::CompileShaders(void)
{
	try {
		if (!_program->IsCompiled())
			_program->CompileAndLink();
	} catch (const std::runtime_error & e) {
		std::cout << e.what() << std::endl;
		if (_program->IsCompute())
			throw std::runtime_error("Failed to compile compute shader");
		
		_program->SetSourceFile("Shaders/Error/Pink.hlsl", VK_SHADER_STAGE_FRAGMENT_BIT);
		_program->SetSourceFile("Shaders/DefaultVertex.hlsl", VK_SHADER_STAGE_VERTEX_BIT);
		_program->CompileAndLink();
	}

	// Retreive set layout of the shader program:

	_setLayouts = _program->GetDescriptorSetLayouts();
}

void					Material::CreatePipelineLayout(void)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	
	pipelineLayoutInfo.setLayoutCount = _setLayouts.size();
	pipelineLayoutInfo.pSetLayouts = _setLayouts.data();

	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout !");
}

void					Material::CreatePipeline(void)
{

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
	pipelineInfo.pStages = _program->GetShaderStages();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = _renderPass->GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &depthStencil;

	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");
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

// Create the default graphic descriptor set layout
void					Material::CreateDescriptorSets(void)
{
	// Find the descriptor layout based on the name of an uniform within it
	VkDescriptorSetLayout		layout = _program->GetDescriptorSetLayout("material");
	VkDescriptorSetAllocateInfo	allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _instance->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = &layout;

	if (vkAllocateDescriptorSets(_device, &allocInfo, &_descriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformPerMaterial.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerMaterial);

    VkDescriptorImageInfo samplerInfo = {};
    samplerInfo.sampler = Vk::Samplers::trilinearClamp;

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	SetBuffer("material", _uniformPerMaterial.buffer, sizeof(LWGC_PerMaterial));
	SetSampler(SamplerBinding::TrilinearClamp, Vk::Samplers::trilinearClamp);
	SetSampler(SamplerBinding::TrilinearRepeat, Vk::Samplers::trilinearRepeat);
	SetSampler(SamplerBinding::NearestClamp, Vk::Samplers::nearestClamp);
	SetSampler(SamplerBinding::NearestRepeat, Vk::Samplers::nearestRepeat);
	SetSampler(SamplerBinding::AnisotropicTrilinearClamp, Vk::Samplers::anisotropicTrilinearClamp);
	SetSampler(SamplerBinding::DepthCompare, Vk::Samplers::depthCompare);

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _descriptorSet;
	descriptorWrites[1].dstBinding = TRILINEAR_CLAMP_BINDING_INDEX;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &samplerInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

// vkDescriptorSet			Material::GetDescriptorSet(void) const
// {
// 	return _descriptorSet;
// }

// VkDescriptorSetLayout	Material::GetDescriptorSetLayout(const std::string & setName) const
// {
// 	return _descriptorSetLayout;
// }

// void					Material::SetDescriptorSetLayout(uint32_t index, VkDescriptorSetLayout layout)
// {
// 	if (_setLayouts.size() <= index)
// 		_setLayouts.resize(index + 1);
	
// 	_setLayouts[index] = layout;
// }

VkPipelineLayout	Material::GetPipelineLayout(void) const
{
	return _pipelineLayout;
}

VkDescriptorSet		Material::GetDescriptorSet(void) const
{
	return _descriptorSet;
}

VkPipeline			Material::GetPipeline(void) const
{
	return _pipeline;
}

uint32_t			Material::GetDescriptorSetBinding(const std::string & setName) const
{
	return _program->GetDescriptorSetBinding(setName);
}

void				Material::SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = size;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = _program->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

// TODO: do not pass the indices but the binding name
void				Material::SetTexture(const std::string & bindingName, const Texture & texture, VkImageLayout imageLayout, VkDescriptorType descriptorType)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
	VkDescriptorImageInfo imageInfo = {};

	imageInfo.imageLayout = imageLayout;
	imageInfo.imageView = texture.GetView();
	imageInfo.sampler = 0;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = _program->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = descriptorType;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void				Material::SetSampler(const std::string & bindingName, VkSampler sampler)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
    VkDescriptorImageInfo samplerInfo = {};

	samplerInfo.sampler = sampler;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptorSet;
	descriptorWrites[0].dstBinding = _program->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &samplerInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "Material" << std::endl;
	(void)r;
	return (o);
}
