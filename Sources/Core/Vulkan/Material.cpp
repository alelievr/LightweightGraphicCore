#include "Material.hpp"

#include <fstream>
#include <array>

#include GLM_INCLUDE
#include GLM_INCLUDE_MATRIX_TRANSFORM

#include "Core/Textures/Texture2DArray.hpp"
#include "Core/Rendering/RenderPipeline.hpp"
#include "Core/Mesh.hpp"
#include "Core/Shaders/BuiltinShaders.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Application.hpp"
#include "Core/ShaderCache.hpp"
#include "Utils/Utils.hpp"

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
	_originalProgram = ShaderCache::GetShader(BuiltinShaders::Pink, BuiltinShaders::DefaultVertex);
	SetupDefaultSettings();
}

Material::Material(const std::string & shader, VkShaderStageFlagBits stage)
{
	_originalProgram = ShaderCache::GetShader(shader, stage);
	SetupDefaultSettings();
}

Material::Material(const std::string & fragmentShader, const std::string & vertexShader)
{
	_originalProgram = ShaderCache::GetShader(fragmentShader, vertexShader);
	SetupDefaultSettings();
}

Material::~Material(void)
{
	// Don't delete if the material have not been initialized
	if (_instance == nullptr)
		return ;

	vkDeviceWaitIdle(_instance->GetDevice());

	CleanupPipeline();

	vkDestroyBuffer(_device, _uniformPerMaterial.buffer, nullptr);
	vkFreeMemory(_device, _uniformPerMaterial.memory, nullptr);
}

Material *Material::Create(void)
{
	return new Material();
}

Material *Material::Create(const std::string & shader, VkShaderStageFlagBits stage)
{
	return new Material(shader, stage);
}

Material *Material::Create(const std::string & fragmentShader, const std::string & vertexShader)
{
	return new Material(fragmentShader, vertexShader);
}

void					Material::SetupDefaultSettings(void)
{
	this->_pipelineLayout = VK_NULL_HANDLE;
	this->_pipeline = VK_NULL_HANDLE;
	this->_bindingTable = nullptr;
	this->_instance = nullptr;
	this->_device = VK_NULL_HANDLE;
	this->_swapChain = nullptr;
	this->_renderPass = nullptr;
	this->_program = nullptr;

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
	Application::Get()->_materialTable.RegsiterObject(this);
}

void					Material::Initialize(SwapChain * swapChain, RenderPass * renderPass)
{
	_instance = VulkanInstance::Get();
	_device = _instance->GetDevice();
	_swapChain = swapChain;
	_renderPass = renderPass;
	_isReady = false;

	CompileShaders();
	CreatePipelineLayout();
	CreatePipeline();
	CreateTextureSampler();
	CreateUniformBuffer();

	// For graphic shaders, we bind default resources
	if (!IsCompute())
	{
		SetBuffer(LWGCBinding::Material, _uniformPerMaterial.buffer, sizeof(LWGC_PerMaterial), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, true);
		SetSampler(SamplerBinding::TrilinearClamp, Vk::Samplers::trilinearClamp, true);
		SetSampler(SamplerBinding::TrilinearRepeat, Vk::Samplers::trilinearRepeat, true);
		SetSampler(SamplerBinding::NearestClamp, Vk::Samplers::nearestClamp, true);
		SetSampler(SamplerBinding::NearestRepeat, Vk::Samplers::nearestRepeat, true);
		SetSampler(SamplerBinding::AnisotropicTrilinearClamp, Vk::Samplers::anisotropicTrilinearClamp, true);
		SetSampler(SamplerBinding::DepthCompare, Vk::Samplers::depthCompare, true);
	}

	// Bind all the properties that have beed bound before the material was ready:
	BindMaterialProperties();
}

void					Material::BindMaterialProperties(void) noexcept
{
	for (const auto & prop : _materialProperties)
	{
		switch (prop.second.propertyType)
		{
			case MaterialPropertyType::Buffer:
				SetBuffer(prop.first, prop.second.buffer, prop.second.size, prop.second.descriptorType);
				break ;
			case MaterialPropertyType::Texture:
				SetTexture(prop.first, prop.second.texture, prop.second.imageLayout, prop.second.descriptorType);
				break ;
			case MaterialPropertyType::Sampler:
				SetSampler(prop.first, prop.second.sampler);
				break ;
			default:
				std::cerr << "Can't bind material property of type " << static_cast< int >(prop.second.propertyType) << std::endl;
				break ;
		}
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
		if (!_originalProgram->IsCompiled())
			_originalProgram->CompileAndLink();
		_program = _originalProgram;
	} catch (const std::runtime_error & e) {
		std::cout << e.what() << std::endl;
		if (IsCompute())
			_program = ShaderCache::GetShader(BuiltinShaders::ComputeError, VK_SHADER_STAGE_COMPUTE_BIT);
		else
			_program = ShaderCache::GetShader(BuiltinShaders::Pink, BuiltinShaders::DefaultVertex);

		if (!_program->IsCompiled())
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

void					Material::ReloadShaders(void) noexcept
{
	CompileShaders();
	vkDestroyPipeline(VulkanInstance::Get()->GetDevice(), _pipeline, NULL);
	CreatePipeline();
}

void					Material::UpdateUniformBuffer()
{
	_perMaterial.albedo = glm::vec4(1, 1, 0, 1);

	Vk::UploadToMemory(_uniformPerMaterial.memory, &_perMaterial, sizeof(_perMaterial));
}

// Allocate the descriptor set from the name of one binding inside the layout
void					Material::AllocateDescriptorSet(const std::string & bindingName)
{
	if (!_program->IsCompiled())
		throw std::runtime_error("Material needs to be compiled before allocating descriptor sets");

	// Generate all descriptor sets from the binding table
	uint32_t setBinding = GetDescriptorSetBinding(bindingName);

	if (setBinding == -1u)
	{
		std::cerr << "Can't allocate descriptor " << bindingName << ": not found in shader" << std::endl;
		return ;
	}

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
		return -1u;

	return _bindingTable->GetDescriptorSetBinding(setName);
}

ShaderProgram *		Material::GetShaderProgram(void) const
{
	return _originalProgram;
}

void				Material::GetComputeWorkSize(uint32_t & width, uint32_t & height, uint32_t & depth) const
{
	_program->GetWorkingThreadSize(width, height, depth);
}

bool				Material::IsCompute(void) const
{
	return (_program != nullptr) ? _program->IsCompute() : _originalProgram->IsCompute();
}

bool					Material::DescriptorSetExists(const std::string & bindingName, bool silent)
{
	MarkAsReady();

	uint32_t setBinding = GetDescriptorSetBinding(bindingName);

	if (setBinding == -1u)
	{
		if (!silent)
			std::cerr << "Can't find binding " << bindingName << " in the shader " << _program->GetName() << ", maybe it was removed at compilation / the shader is not compiled" << std::endl;
		return false;
	}

	// Do nothing if the descriptor is already allocated, otherwise allocate it
	AllocateDescriptorSet(bindingName);
	return true;
}

void				Material::SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType, bool silent)
{
	// Store the material properties so when we re-create it we cant set these values
	_materialProperties[bindingName] = MaterialProperty{
		MaterialPropertyType::Buffer,
		descriptorType,
		buffer,
		size,
		nullptr,
		{},
		VK_NULL_HANDLE
	};

	if (!IsCompiled())
		return ;
	if (!DescriptorSetExists(bindingName, silent))
		return ;

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

void				Material::SetTexture(const std::string & bindingName, const Texture * texture, VkImageLayout imageLayout, VkDescriptorType descriptorType, bool silent)
{

	_materialProperties[bindingName] = MaterialProperty{
		MaterialPropertyType::Texture,
		descriptorType,
		VK_NULL_HANDLE,
		0,
		texture,
		imageLayout,
		VK_NULL_HANDLE
	};

	if (!IsCompiled())
		return ;
	if (!DescriptorSetExists(bindingName, silent))
		return ;

	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
	VkDescriptorImageInfo imageInfo = {};

	imageInfo.imageLayout = imageLayout;
	imageInfo.imageView = texture->GetView();
	imageInfo.sampler = 0;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _setTable.at(_bindingTable->GetDescriptorSetBinding(bindingName)).set;
	descriptorWrites[0].dstBinding = _bindingTable->GetDescriptorIndex(bindingName);
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = descriptorType;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void				Material::SetSampler(const std::string & bindingName, VkSampler sampler, bool silent)
{
	_materialProperties[bindingName] = MaterialProperty{
		MaterialPropertyType::Sampler,
		VK_DESCRIPTOR_TYPE_SAMPLER,
		VK_NULL_HANDLE,
		0,
		nullptr,
		{},
		sampler
	};

	if (!IsCompiled())
		return ;
	if (!DescriptorSetExists(bindingName, silent))
		return ;

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

bool				Material::IsReady(void) const noexcept { return _isReady; }

bool				Material::IsInitialized(void) const
{
	return (_instance != nullptr);
}

bool				Material::IsCompiled(void) const noexcept { return _program != nullptr && _program->IsCompiled(); }

void				Material::MarkAsReady(void) noexcept
{
	if (IsInitialized())
		return ;

	_isReady = true;
	Application::Get()->GetMaterialTable()->NotifyMaterialReady(this);
}

std::ostream &	operator<<(std::ostream & o, Material const & r)
{
	o << "Material" << std::endl;
	(void)r;
	return (o);
}
