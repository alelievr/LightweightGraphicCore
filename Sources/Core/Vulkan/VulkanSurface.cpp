#include "VulkanSurface.hpp"

using namespace LWGC;

VulkanSurface::VulkanSurface(void)
{
	this->_window = NULL;
	this->_surface = VK_NULL_HANDLE;
}

VulkanSurface::~VulkanSurface(void)
{
	vkDestroySurfaceKHR(_instance->GetInstance(), _surface, nullptr);
}

void		VulkanSurface::Initialize(GLFWwindow * window)
{
	_instance = VulkanInstance::Get();
	_window = window;

	if (glfwCreateWindowSurface(_instance->GetInstance(), window, nullptr, &_surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");

	_instance->InitializeSurface(_surface);
}

GLFWwindow *		VulkanSurface::GetWindow(void) const noexcept { return (this->_window); }
VkSurfaceKHR		VulkanSurface::GetSurface(void) const noexcept { return (this->_surface); }

std::ostream &	operator<<(std::ostream & o, VulkanSurface const & r)
{
	o << "Vulkan Surface" << std::endl;
	(void)r;
	return (o);
}
