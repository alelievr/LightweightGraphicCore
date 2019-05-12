#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "WindowFlag.hpp"
#include "Rendering/RenderPipeline.hpp"
#include "EventSystem.hpp"
#include "Hierarchy.hpp"
#include "Vulkan/VulkanInstance.hpp"
#include "Vulkan/VulkanSurface.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/RenderPass.hpp"
#include "Vulkan/Material.hpp"
#include "Core/Rendering/RenderContext.tpp"
#include "Core/Delegate.tpp"
#include "Core/ImGUIWrapper.hpp"
#include "Core/MaterialTable.hpp"
#include "Core/Textures/TextureTable.hpp"
#include "Core/Textures/Texture2D.hpp"
#include "Core/Textures/Texture2DArray.hpp"
#include "Core/Rendering/RenderPipelineManager.hpp"

#include VULKAN_INCLUDE
#include GLFW_INCLUDE

namespace LWGC
{
	enum class AppCapability
	{
		Default				= 0x000,
		RayTracing			= 0x001,
		CooperativeMatrices	= 0x002,
	};

	class		Application
	{
		friend class Texture;
		friend class Material;
		private:
			static Application *				_app;
			VulkanInstance						_instance;
			VulkanSurface						_surface;
			SwapChain							_swapChain;
			GLFWwindow *						_window;
			EventSystem							_eventSystem;
			std::shared_ptr< Hierarchy >		hierarchy;
			ImGUIWrapper						_imGUI;
			MaterialTable						_materialTable;
			TextureTable						_textureTable;
			bool								_shouldNotQuit;

			void		UpdateRenderPipeline(void);

			static void	FramebufferResizeCallback(GLFWwindow *window, int width, int height);

		public:
			Application(bool initDeafultRenderPipeline = true);
			Application(const Application&) = delete;
			virtual ~Application(void);

			Application &	operator=(Application const & src) = delete;

			void	Init(const AppCapability capabilities = AppCapability::Default) noexcept;
			bool	ShouldNotQuit(void) const noexcept;
			void	Quit(void) noexcept;
			void	Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept;
			void	Update(void) noexcept;

			SwapChain *			GetSwapChain(void) noexcept;
			EventSystem *		GetEventSystem(void) noexcept;
			Hierarchy *			GetHierarchy(void) noexcept;
			MaterialTable *		GetMaterialTable(void) noexcept;
			TextureTable *		GetTextureTable(void) noexcept;

			static Application *		Get(void) noexcept;

			// public events
			static Delegate< void(void) >	update;
			static Delegate< void(void) >	lateUpdate;
	};

	std::ostream &	operator<<(std::ostream & o, Application const & r);
}
