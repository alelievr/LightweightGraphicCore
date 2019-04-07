#include "Camera.hpp"

#include "Core/Vulkan/Vk.hpp"
#include "Core/Application.hpp"
#include "Core/Rendering/RenderPipeline.hpp"
#include "Utils/Vector.hpp"
#include "IncludeDeps.hpp"
#include GLM_INCLUDE_QUATERNION
#include GLM_INCLUDE_MATRIX_TRANSFORM

using namespace LWGC;

Camera::Camera(void)
{
	this->_target = new RenderTarget();
	this->_viewportSize = glm::vec2(0, 0);
	this->_cameraType = CameraType::Perspective;
	this->_fov = 60;
	this->_nearPlane = 0.1f;
	this->_farPlane = 1e10;
}

Camera::~Camera(void)
{
	auto device = VulkanInstance::Get()->GetDevice();

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

	Vk::CreateBuffer(
		sizeof(LWGC_PerCamera),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformCameraBuffer.buffer,
		_uniformCameraBuffer.memory
	);
	
	_perCameraSet.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _uniformCameraBuffer.buffer, sizeof(LWGC_PerCamera));
}

glm::mat4 Camera::ReverseZPerspective(float fovy, float aspect, float zNear, float zFar)
{
	assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

	float const tanHalfFovy = tan(fovy / 2.0f);
	
	glm::mat4 Result(0.0f);
	Result[0][0] = 1.0f / (aspect * tanHalfFovy);
	Result[1][1] = 1.0f / (tanHalfFovy);
	Result[2][3] = 1.0f;

	Result[2][2] = -(zFar / (zNear - zFar)) - 1.0f;
	Result[3][2] = -(zNear * zFar) / (zNear - zFar);

	return Result;
}

void					Camera::UpdateUniformData(void) noexcept
{
	_perCamera.positionWS = glm::vec4(transform->GetPosition(), 1.0f);

	// World to camera is the inverse of local (camera) to world
	_perCamera.view = glm::inverse(transform->GetLocalToWorldMatrix());

	auto swapChainExtent = Application::Get()->GetSwapChain()->GetExtent();
	_viewportSize.x = swapChainExtent.width;
	_viewportSize.y = swapChainExtent.height;

	float ratio = _viewportSize.x / _viewportSize.y;
	_perCamera.projection = ReverseZPerspective(glm::radians(_fov), ratio, _nearPlane, _farPlane);

	// Invert y because Opengl
	_perCamera.projection[1][1] *= -1;

	// Transpose for HLSL
	_perCamera.projection = glm::transpose(_perCamera.projection);
	_perCamera.view = glm::transpose(_perCamera.view);
	_perCamera.screenSize = glm::vec4(_viewportSize, 1.0f / _viewportSize);

	Vk::UploadToMemory(_uniformCameraBuffer.memory, &_perCamera, sizeof(_perCamera));
}

VkDescriptorSet			Camera::GetDescriptorSet(void)
{
	return _perCameraSet.GetDescriptorSet();
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
