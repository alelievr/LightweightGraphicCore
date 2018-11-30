#include "Camera.hpp"

#include "Core/Vulkan/Vk.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "IncludeDeps.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include GLM_INCLUDE_QUATERNION
#include GLM_INCLUDE_MATRIX_TRANSFORM

using namespace LWGC;

VkDescriptorSetLayout Camera::_perCameraDescriptorSetLayout = VK_NULL_HANDLE;

Camera::Camera(void) : _initDescriptorSetLayout(false)
{
	std::cout << "Default constructor of Camera called" << std::endl;
	this->_target = new RenderTarget();
	this->_cameraType = CameraType::Perspective;
	this->_fov = 60;
	this->_nearPlane = 0.001f;
	this->_farPlane = 1e10;

	if (_perCameraDescriptorSetLayout == VK_NULL_HANDLE)
	{
		_initDescriptorSetLayout = true;
	}
}

Camera::~Camera(void)
{
	auto device = VulkanInstance::Get()->GetDevice();

	if (_initDescriptorSetLayout)
		vkDestroyDescriptorSetLayout(device, _perCameraDescriptorSetLayout, nullptr);

	vkDestroyBuffer(device, _uniformCameraBuffer.buffer, nullptr);
	vkFreeMemory(device, _uniformCameraBuffer.memory, nullptr);
}

void			Camera::OnEnable(void) noexcept
{
	Component::OnEnable();
}

void			Camera::OnDisable(void) noexcept
{
	Component::OnDisable();
}

void					Camera::Update(void) noexcept
{
	UpdateUniformData();
}

glm::vec3		Camera::WorldToScreenPoint(glm::vec3 worldPosition)
{
	return worldPosition;
}

glm::vec3		Camera::ScreenToWorldPoint(glm::vec3 screenPosition)
{
	return screenPosition;
}

RenderTarget *	Camera::GetTarget(void) const { return (this->_target); }
void		Camera::SetTarget(RenderTarget * tmp) { this->_target = tmp; }

glm::vec2	Camera::GetSize(void) const { return (this->_size); }
void		Camera::SetSize(glm::vec2 tmp) { this->_size = tmp; }

CameraType	Camera::GetCameraType(void) const { return (this->_cameraType); }
void		Camera::SetCameraType(CameraType tmp) { this->_cameraType = tmp; }

float		Camera::GetFov(void) const { return (this->_fov); }
void		Camera::SetFov(float tmp) { this->_fov = tmp; }

float		Camera::GetNearPlane(void) const { return (this->_nearPlane); }
void		Camera::SetNearPlane(float tmp) { this->_nearPlane = tmp; }

float		Camera::GetFarPlane(void) const { return (this->_farPlane); }
void		Camera::SetFarPlane(float tmp) { this->_farPlane = tmp; }

void		Camera::Initialize(void) noexcept
{
	Component::Initialize();

	_swapChain = VulkanRenderPipeline::Get()->GetSwapChain();
	
	if (_initDescriptorSetLayout == VK_NULL_HANDLE)
		CreateCameraDescriptorSetLayout();

	Vk::CreateBuffer(
		sizeof(LWGC_PerCamera),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformCameraBuffer.buffer,
		_uniformCameraBuffer.memory
	);

	CreateDescriptorSet();
}

void					Camera::CreateDescriptorSet(void) noexcept
{
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = VulkanInstance::Get()->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = &_perCameraDescriptorSetLayout;

	if (vkAllocateDescriptorSets(device, &allocInfo, &_perCameraDescriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniformCameraBuffer.buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(LWGC_PerCamera);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _perCameraDescriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

void					Camera::UpdateUniformData(void) noexcept
{
	_perCamera.positionWS = glm::vec4(transform->GetPosition(), 1.0f);

	// view from positon and rotation
	_perCamera.view = glm::translate(glm::toMat4(transform->GetRotation()), -transform->GetPosition());

	float ratio = (float)_swapChain->GetExtent().width / (float)_swapChain->GetExtent().height;
	_perCamera.projection = glm::perspective(glm::radians(_fov), ratio, _nearPlane, _farPlane);

	// Invert y because opengl
	_perCamera.projection[1][1] *= -1;
 
	void* data;
	vkMapMemory(device, _uniformCameraBuffer.memory, 0, sizeof(LWGC_PerCamera), 0, &data);
	memcpy(data, &_perCamera, sizeof(_perCamera));
	vkUnmapMemory(device, _uniformCameraBuffer.memory);
}

void					Camera::CreateCameraDescriptorSetLayout(void) noexcept
{
	auto binding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
	Vk::CreateDescriptorSetLayout({binding}, _perCameraDescriptorSetLayout);
}

void					Camera::BindDescriptorSet(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint)
{
	vkCmdBindDescriptorSets(cmd, bindPoint, Vk::currentPipelineLayout, PER_CAMERA_BINDING_INDEX, 1, &_perCameraDescriptorSet, 0, nullptr);
}

VkDescriptorSetLayout	Camera::GetDescriptorSetLayout(void) noexcept
{
	if (_perCameraDescriptorSetLayout == VK_NULL_HANDLE)
		CreateCameraDescriptorSetLayout();

	return _perCameraDescriptorSetLayout;
}

uint32_t	Camera::GetType(void) const noexcept
{
	return type;
}

std::ostream &	operator<<(std::ostream & o, Camera const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
