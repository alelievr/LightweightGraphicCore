#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "WindowFlag.hpp"
#include "Rendering/VulkanRenderPipeline.hpp"
#include "EventSystem.hpp"
#include "Hierarchy.hpp"
#include "Vulkan/VulkanInstance.hpp"
#include "Vulkan/VulkanSurface.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/RenderPass.hpp"
#include "Vulkan/Material.hpp"
#include "Core/Rendering/RenderContext.hpp"
#include "Core/Delegate.tpp"
#include "Core/ImGUIWrapper.hpp"
#include "Core/MaterialTable.hpp"

#include VULKAN_INCLUDE
#include GLFW_INCLUDE

namespace LWGC
{
	class		Application
	{
		friend class Material;
		private:
			static Application *				_app;
			VulkanInstance						_instance;
			VulkanSurface						_surface;
			SwapChain							_swapChain;
			VulkanRenderPipeline *				_renderPipeline;
			GLFWwindow *						_window;
			EventSystem							_eventSystem;
			std::shared_ptr< Hierarchy >		hierarchy;
			// ImGUIWrapper						_imGUI;
			MaterialTable						_materialTable;

			bool		_shouldNotQuit;

			static void	FramebufferResizeCallback(GLFWwindow *window, int width, int height);

		public:
			Application(void);
			Application(VulkanRenderPipeline * renderPipeline);
			Application(const Application&) = delete;
			virtual ~Application(void);

			Application &	operator=(Application const & src) = delete;

			void	Init(void) noexcept;
			bool	ShouldNotQuit(void) const noexcept;
			void	Quit(void) noexcept;
			void	Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept;
			void	Update(void) noexcept;

			EventSystem *		GetEventSystem(void) noexcept;
			Hierarchy *			GetHierarchy(void) noexcept;
			MaterialTable *		GetMaterialTable(void) noexcept;

			static Application *		Get(void) noexcept;

			// public events
			static Delegate< void(void) >	update;
			static Delegate< void(void) >	lateUpdate;
	};

	std::ostream &	operator<<(std::ostream & o, Application const & r);
}
