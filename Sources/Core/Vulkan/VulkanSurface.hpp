#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE
#include GLFW_INCLUDE
#include "VulkanInstance.hpp"

namespace LWGC
{
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
}