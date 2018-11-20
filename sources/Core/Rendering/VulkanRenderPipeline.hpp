#pragma once

#include <array>

#include "RenderTarget.hpp"
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
			static VulkanRenderPipeline *	pipelineInstance;
			std::vector< VkCommandBuffer >	swapChainCommandBuffers;
			
			void				RenderInternal(const std::vector< Camera * > & cameras, RenderContext & context);

		protected:
			std::vector<VkSemaphore>		imageAvailableSemaphores;
			std::vector<VkSemaphore>		renderFinishedSemaphores;
			std::vector<VkFence>			inFlightFences;
			size_t					currentFrame = 0;
			RenderPass				renderPass;
			SwapChain *				swapChain;
			VulkanInstance *		instance;
			VkCommandBuffer			graphicCommandBuffer;
			bool					framebufferResized;
			
			virtual void		CreateRenderPass(void);
			void				BeginRenderPass(void);
			void				EndRenderPass(void);
			virtual void		RecreateSwapChain(RenderContext & renderContext);
			virtual void		Render(const std::vector< Camera * > & cameras, RenderContext & context) = 0;

		public:
			VulkanRenderPipeline(void);
			VulkanRenderPipeline(const VulkanRenderPipeline & p) = delete;
			virtual			~VulkanRenderPipeline(void);

			VulkanRenderPipeline & operator=(const VulkanRenderPipeline & rhs) = delete;

			virtual void	Initialize(SwapChain * swapChain);
			virtual void	CreateSyncObjects(void);

			SwapChain *		GetSwapChain(void);
			RenderPass *	GetRenderPass(void);

			static VulkanRenderPipeline *	Get();
	};
}
