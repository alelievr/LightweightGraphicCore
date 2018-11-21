#include "Camera.hpp"

#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

Camera::Camera(void)
{
	std::cout << "Default constructor of Camera called" << std::endl;
	this->_target = new RenderTarget();
	this->_cameraType = CameraType::Perspective;
	this->_fov = 0;
	this->_nearPlane = 0;
	this->_farPlane = 0;
}

Camera::~Camera(void)
{
	vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);

	std::cout << "Destructor of Camera called" << std::endl;
}

void OnAdded(const GameObject & go) noexcept
{
}

void OnRemoved(const GameObject & go) noexcept
{
	(void)go;
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
	Vk::CreateBuffer(
		sizeof(LWGC_PerCamera),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniformCameraBuffer.buffer,
		_uniformCameraBuffer.memory
	);
}

VkDescriptorSetLayout	Camera::GetDescriptorSetLayout(void) const noexcept
{
	return _descriptorSetLayout;
}

uint32_t	Camera::GetType(void) const
{
	return type;
}

std::ostream &	operator<<(std::ostream & o, Camera const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
