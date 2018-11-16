#pragma once

#include <array>

#include "RenderTarget.hpp"
#include "IRenderable.hpp"
#include "Core/Components/Camera.hpp"
#include "RenderContext.hpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/SwapChain.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Mesh.hpp"

#define MAX_FRAMES_IN_FLIGHT 2

namespace LWGC
{
	class VulkanRenderPipeline
	{
		friend class Application;
		
		// The private part is only used as internal render-pipeline setup and should be overwritten by a custom render pipeline
		private:
			// TODO: move material into the MeshRenderer
			std::vector<VkSemaphore> imageAvailableSemaphores;
			std::vector<VkSemaphore> renderFinishedSemaphores;
			std::vector<VkFence> inFlightFences;
			size_t currentFrame = 0;

			std::vector<VkCommandBuffer> commandBuffers;
			
			Mesh				mesh;
			Material			material;

		protected:
			RenderPass			renderPass;
			SwapChain *			swapChain;
			VulkanInstance *	instance;
			CommandBufferPool *	graphicCommandBufferPool;
			bool				framebufferResized;
			
			virtual void		CreateRenderPass(void);

		public:
			VulkanRenderPipeline(void);
			VulkanRenderPipeline(const VulkanRenderPipeline & p) = delete;
			virtual		~VulkanRenderPipeline(void);

			VulkanRenderPipeline & operator=(const VulkanRenderPipeline & rhs) = delete;

			virtual void	Initialize(SwapChain * swapChain);
			virtual void	Render(const std::vector< Camera * > & cameras, const RenderContext & context) = 0;
			virtual void	CreateMeshes(void);
			virtual void	PrepareCommandBuffers(void);
			virtual void	CreateSyncObjects(void);
			virtual void	RecreateSwapChain(void);
	};
}
