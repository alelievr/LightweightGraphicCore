#pragma once

#include <iostream>
#include <string>

#include "WindowFlag.hpp"
#include "Rendering/VulkanRenderPipeline.hpp"
#include "Events/EventSystem.hpp"
#include "Hierarchy.hpp"
#include "Vulkan/VulkanInstance.hpp"
#include "Vulkan/VulkanSurface.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/RenderPass.hpp"
#include "Vulkan/Material.hpp"

#include VULKAN_INCLUDE
#include GLFW_INCLUDE

namespace LWGC
{
	class		Application
	{
		private:
			std::shared_ptr< VulkanRenderPipeline >	_renderPipeline;
			EventSystem							_eventSystem;
			Hierarchy							_hierarchy;
			GLFWwindow *						_window;
			VulkanInstance						_instance;
			VulkanSurface						_surface;
			SwapChain							_swapChain;

			bool	_shouldNotQuit;
			bool	_framebufferResized;

			static void			Application::FramebufferResizeCallback(GLFWwindow *window, int width, int height);

		public:
			Application(void);
			Application(const Application&) = delete;
			virtual ~Application(void);

			Application &	operator=(Application const & src) = delete;

			void	Init(void) noexcept;
			bool	ShouldNotQuit(void) const noexcept;
			void	Quit(void) noexcept;
			void	Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept;
			void	Update(void) noexcept;

			VulkanRenderPipeline &	GetRenderPipeline(void) const noexcept;
			void	SetRenderPipeline(std::shared_ptr< VulkanRenderPipeline > tmp) noexcept;
			
			EventSystem *		GetEventSystem(void) noexcept;

			Hierarchy *			GetHierarchy(void) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Application const & r);
}
