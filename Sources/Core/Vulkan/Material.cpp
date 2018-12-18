#include "Material.hpp"

#include <fstream>
#include <array>

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

const std::string	LWGCBinding::Frame = "frame";
const std::string	LWGCBinding::Camera = "camera";
const std::string	LWGCBinding::Material = "material";
const std::string	LWGCBinding::Object = "object";

Material::Material(void)
{
	this->_pipelineLayout = VK_NULL_HANDLE;
	this->_pipeline = VK_NULL_HANDLE;
	this->_bindingTable = nullptr;
	_program = new ShaderProgram();
	_program->SetSourceFile(BuiltinShaders::Pink, VK_SHADER_STAGE_FRAGMENT_BIT);
	_program->SetSourceFile(BuiltinShaders::DefaultVertex, VK_SHADER_STAGE_VERTEX_BIT);
	SetupDefaultSettings();
}

Material::Material(const std::string & shader, VkShaderStageFlagBits stage)
{
	_program = new ShaderProgram();
	_program->SetSourceFile(shader, stage);
	this->_bindingTable = nullptr;
	SetupDefaultSettings();
}

Material::Material(const std::string & fragmentShader, const std::string & vertexShader)
{
	_program = new ShaderProgram();
	_program->SetSourceFile(fragmentShader, VK_SHADER_STAGE_FRAGMENT_BIT);
	_program->SetSourceFile(vertexShader, VK_SHADER_STAGE_VERTEX_BIT);
	this->_bindingTable = nullptr;
	SetupDefaultSettings();
}

Material::Material(ShaderProgram * program)
{
	_program = program;
	SetupDefaultSettings();
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

	// TODO: destroy samplers from Vk::Samplers
	// vkDestroySampler(_device, _samplers[0], nullptr);

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

void					Material::SetupDefaultSettings(void)
{
	static auto bindingDescription = Mesh::GetBindingDescription();
	static auto attributeDescriptions = Mesh::GetAttributeDescriptions();

	_vertexInputState = {};
	_vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vertexInputState.vertexBindingDescriptionCount = bindingDescription.size();
	_vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	_vertexInputState.pVertexBindingDescriptions = bindingDescription.data();
	_vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

	_inputAssemblyState = {};
	_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	_inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	_depthStencilState = {};
	_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	_depthStencilState.depthTestEnable = VK_TRUE;
	_depthStencilState.depthWriteEnable = VK_TRUE;
	_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
	_depthStencilState.depthBoundsTestEnable = VK_FALSE;
	_depthStencilState.minDepthBounds = 0.0f; // Optional
	_depthStencilState.maxDepthBounds = 1.0f; // Optional
	_depthStencilState.stencilTestEnable = VK_FALSE;
	_depthStencilState.front = {}; // Optional
	_depthStencilState.back = {}; // Optional

	_rasterizationState = {};
	_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	_rasterizationState.depthClampEnable = VK_FALSE;
	_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	_rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	_rasterizationState.lineWidth = 1.0f;
	_rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
	_rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	_rasterizationState.depthBiasEnable = VK_FALSE;
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
	
	// For graphic shaders, we bind default resources
	if (!IsCompute())
	{
		AllocateDescriptorSet(LWGCBinding::Material);
		SetBuffer(LWGCBinding::Material, _uniformPerMaterial.buffer, sizeof(LWGC_PerMaterial), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		SetSampler(SamplerBinding::TrilinearClamp, Vk::Samplers::trilinearClamp);
		SetSampler(SamplerBinding::TrilinearRepeat, Vk::Samplers::trilinearRepeat);
		SetSampler(SamplerBinding::NearestClamp, Vk::Samplers::nearestClamp);
		SetSampler(SamplerBinding::NearestRepeat, Vk::Samplers::nearestRepeat);
		SetSampler(SamplerBinding::AnisotropicTrilinearClamp, Vk::Samplers::anisotropicTrilinearClamp);
		SetSampler(SamplerBinding::DepthCompare, Vk::Samplers::depthCompare);
	}
}

void					Material::CleanupPipeline(void) noexcept
{
	vkDestroyPipeline(_device, _pipeline, nullptr);
	vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
}

void					Material::CompileShaders(void)
{
	try {
		if (!_program->IsCompiled())
			_program->CompileAndLink();
	} catch (const std::runtime_error & e) {
		std::cout << e.what() << std::endl;
		if (IsCompute())
			throw std::runtime_error("Failed to compile compute shader");
		
		_program->SetSourceFile(BuiltinShaders::Pink, VK_SHADER_STAGE_FRAGMENT_BIT);
		_program->SetSourceFile(BuiltinShaders::DefaultVertex, VK_SHADER_STAGE_VERTEX_BIT);
		_program->CompileAndLink();
	}

	// Retreive set layout of the shader program:
	_bindingTable = _program->GetShaderBindingTable();
	_setLayouts = _bindingTable->GetDescriptorSetLayouts();
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
	if (_program->IsCompute())
		CreateComputePipeline();
	else
		CreateGraphicPipeline();
}

void					Material::CreateComputePipeline(void)
{
	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = _program->GetShaderStages()[0];
	pipelineCreateInfo.layout = _pipelineLayout;

	Vk::CheckResult(vkCreateComputePipelines(
		_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &_pipeline),
		"Can't create compute pipeline");
}

void					Material::CreateGraphicPipeline(void)
{
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

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = _program->GetShaderStages();
	pipelineInfo.pVertexInputState = &_vertexInputState;
	pipelineInfo.pInputAssemblyState = &_inputAssemblyState;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &_rasterizationState;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = _renderPass->GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &_depthStencilState;

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

// Allocate the descriptor set from the name of one binding inside the layout
void					Material::AllocateDescriptorSet(const std::string & bindingName)
{
	if (!_program->IsCompiled())
		throw std::runtime_error("Material needs to be compiled before allocating descriptor sets");
	
	// Generate all descriptor sets from the binding table
	uint32_t setBinding = _bindingTable->GetDescriptorSetBinding(bindingName);

	// Allocate the binding if not already allocated
	if (_setTable.find(setBinding) == _setTable.end())
	{
		VkDescriptorSetLayout	layout = _bindingTable->GetDescriptorSetLayout(bindingName);
		VulkanInstance *		instance = VulkanInstance::Get();
		VkDescriptorSet			set;

		VkDescriptorSetAllocateInfo	allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = instance->GetDescriptorPool();
		allocInfo.descriptorSetCount = 1u;
		allocInfo.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(instance->GetDevice(), &allocInfo, &set) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
		_setTable[setBinding] = {set, bindingName};
	}
}

VkPipelineLayout	Material::GetPipelineLayout(void) const
{
	return _pipelineLayout;
}

VkPipeline			Material::GetPipeline(void) const
{
	return _pipeline;
}

uint32_t			Material::GetDescriptorSetBinding(const std::string & setName) const
{
	// We ignore nonexistent bindings
	if (!_program->HasBinding(setName))
		return -1;
	
	return _bindingTable->GetDescriptorSetBinding(setName);
}

bool				Material::IsCompute(void) const
{
	return _program->IsCompute();
}

void				Material::SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = size;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _setTable.at(_bindingTable->GetDescriptorSetBinding(bindingName)).set;
	descriptorWrites[0].dstBinding = _bindingTable->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = descriptorType;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(_device, static_cast< uint32_t >(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void				Material::SetTexture(const std::string & bindingName, const Texture & texture, VkImageLayout imageLayout, VkDescriptorType descriptorType)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
	VkDescriptorImageInfo imageInfo = {};

	imageInfo.imageLayout = imageLayout;
	imageInfo.imageView = texture.GetView();
	imageInfo.sampler = 0;

	std::cout << "Bind texture: " << bindingName << ": " << _bindingTable->GetDescriptorIndex(bindingName) << std::endl;;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _setTable.at(_bindingTable->GetDescriptorSetBinding(bindingName)).set;
	descriptorWrites[0].dstBinding = _bindingTable->GetDescriptorIndex(bindingName);
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
	descriptorWrites[0].dstSet = _setTable.at(_bindingTable->GetDescriptorSetBinding(bindingName)).set;
	descriptorWrites[0].dstBinding = _bindingTable->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &samplerInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void				Material::BindDescriptorSets(RenderPass * renderPass)
{
	for (const auto & k : _setTable)
		renderPass->BindDescriptorSet(k.second.name, k.second.set);
}

void				Material::SetVertexInputState(VkPipelineVertexInputStateCreateInfo info) { _vertexInputState = info; }
void				Material::SetInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo info) { _inputAssemblyState = info; }
void				Material::SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo info) { _depthStencilState = info; }
void				Material::SetRasterizationState(VkPipelineRasterizationStateCreateInfo info) { _rasterizationState = info; }

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "Material" << std::endl;
	(void)r;
	return (o);
}
