#pragma once

#include <iostream>
#include <string>
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Vulkan/VulkanSurface.hpp"
#include "Core/Vulkan/RenderPass.hpp"

#include VULKAN_INCLUDE
#include IMGUI_INCLUDE
#include IMGUI_GLFW_INCLUDE
#include IMGUI_VULKAN_INCLUDE

namespace LWGC
{
	class		ImGUIWrapper
	{
		private:
			VulkanInstance *	_instance;
			VkDevice			_device;
			VkDescriptorPool	_descriptorPool;
			SwapChain *			_swapChain;
			VulkanSurface *		_surface;
			VkQueue				_queue;
			RenderPass			_renderPass;
			ImGui_ImplVulkanH_WindowData	_wd;

			void		CreateDescriptorPool(void);
			void		InitImGUI(void);
			void		UploadFonts(void);
			void		InitImGUIFrameDatas(void);

		public:
			ImGUIWrapper(void);
			ImGUIWrapper(const ImGUIWrapper &) = delete;
			virtual ~ImGUIWrapper(void);

			void		Initialize(SwapChain * swapChain, VulkanSurface * surface);
			void		BeginFrame(void);
			void		EndFrame(void);
			void		UpdatePipelineDependentDatas(void);

			ImGUIWrapper &	operator=(ImGUIWrapper const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, ImGUIWrapper const & r);
}
