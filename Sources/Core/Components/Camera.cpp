#include "Camera.hpp"

#include "Core/Vulkan/Vk.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Utils/Vector.hpp"
#include "IncludeDeps.hpp"
#include GLM_INCLUDE_QUATERNION
#include GLM_INCLUDE_MATRIX_TRANSFORM

using namespace LWGC;

VkDescriptorSetLayout Camera::_perCameraDescriptorSetLayout = VK_NULL_HANDLE;

Camera::Camera(void) : _swapChain(nullptr), _initDescriptorSetLayout(false)
{
	this->_target = new RenderTarget();
	this->_viewportSize = glm::vec2(0, 0);
	this->_cameraType = CameraType::Perspective;
	this->_fov = 60;
	this->_nearPlane = 0.1f;
	this->_farPlane = 1e10;

	if (_perCameraDescriptorSetLayout != VK_NULL_HANDLE)
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

// return NDC space
glm::vec3		Camera::WorldToScreenPoint(glm::vec3 worldPosition)
{
	glm::vec3 cameraSpacePos = GetViewMatrix() * glm::vec4(worldPosition, 1);
	glm::vec3 ndcPos = GetProjectionMatrix() * glm::vec4(cameraSpacePos, 1);

	// remove perspective
	ndcPos.x /= ndcPos.z;
	ndcPos.y /= ndcPos.z;
	ndcPos.z = 0;

	return ndcPos;
}

glm::vec3		Camera::ScreenToWorldPoint(glm::vec3 screenPosition)
{
	std::cout << "ScreenToWorldPoint: TODO !" << std::endl;
	return screenPosition;
}

void		Camera::Initialize(void) noexcept
{
	Component::Initialize();

	_swapChain = VulkanRenderPipeline::Get()->GetSwapChain();

	_viewportSize.x = _swapChain->GetExtent().width;
	_viewportSize.y = _swapChain->GetExtent().height;

	if (_initDescriptorSetLayout == false)
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

void					Camera::CreateDescriptorSet(void)
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

	// World to camera is the inverse of local (camera) to world
	_perCamera.view = glm::inverse(transform->GetLocalToWorldMatrix());

	_viewportSize.x = _swapChain->GetExtent().width;
	_viewportSize.y = _swapChain->GetExtent().height;

	float ratio = _viewportSize.x / _viewportSize.y;
	_perCamera.projection = glm::perspective(glm::radians(_fov), ratio, _nearPlane, _farPlane);

	// Invert y because Opengl
	_perCamera.projection[1][1] *= -1;

	// Transpose for HLSL
	_perCamera.projection = glm::transpose(_perCamera.projection);
	_perCamera.view = glm::transpose(_perCamera.view);

	Vk::UploadToMemory(_uniformCameraBuffer.memory, &_perCamera, sizeof(_perCamera));
}

void					Camera::CreateCameraDescriptorSetLayout(void) noexcept
{
	auto binding = Vk::CreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS);
	Vk::CreateDescriptorSetLayout({binding}, _perCameraDescriptorSetLayout);
}

VkDescriptorSet			Camera::GetDescriptorSet(void) const
{
	return _perCameraDescriptorSet;
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

RenderTarget *	Camera::GetTarget(void) const { return (this->_target); }
void		Camera::SetTarget(RenderTarget * tmp) { this->_target = tmp; }

glm::vec2	Camera::GetViewportSize(void) const { return (this->_viewportSize); }

CameraType	Camera::GetCameraType(void) const { return (this->_cameraType); }
void		Camera::SetCameraType(CameraType tmp) { this->_cameraType = tmp; }

float		Camera::GetFov(void) const { return (this->_fov); }
void		Camera::SetFov(float tmp) { this->_fov = tmp; }

float		Camera::GetNearPlane(void) const { return (this->_nearPlane); }
void		Camera::SetNearPlane(float tmp) { this->_nearPlane = tmp; }

float		Camera::GetFarPlane(void) const { return (this->_farPlane); }
void		Camera::SetFarPlane(float tmp) { this->_farPlane = tmp; }

// Untranspose the matrix because they was encoded for HLSL
glm::mat4	Camera::GetViewMatrix(void) const { return glm::transpose(_perCamera.view); }
glm::mat4	Camera::GetProjectionMatrix(void) const { return glm::transpose(_perCamera.projection); }

std::ostream &	operator<<(std::ostream & o, Camera const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
