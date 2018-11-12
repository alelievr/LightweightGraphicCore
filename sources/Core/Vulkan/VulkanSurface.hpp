#pragma once

#include <iostream>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "VulkanInstance.hpp"

class		VulkanSurface
{
	private:
		GLFWwindow *		_window;
		VkSurfaceKHR		_surface;
		VulkanInstance *	_instance;

	public:
		VulkanSurface(void);
		VulkanSurface(const VulkanSurface &) = delete;
		virtual ~VulkanSurface(void);

		VulkanSurface &	operator=(VulkanSurface const & src) = delete;

		void			Initialize(GLFWwindow * window);

		GLFWwindow *	GetWindow(void) const noexcept;
		VkSurfaceKHR	GetSurface(void) const noexcept;
};

std::ostream &	operator<<(std::ostream & o, VulkanSurface const & r);
