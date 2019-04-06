#pragma once

#include <array>

#include "RenderTarget.hpp"
#include "Core/Components/Camera.hpp"
#include "RenderContext.tpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/CommandBufferPool.hpp"
#include "Core/Rendering/IRenderQueue.hpp"
#include "Core/Vulkan/DescriptorSet.hpp"

#include IMGUI_INCLUDE

#define PER_FRAME_BINDING_INDEX	0

namespace LWGC
{
	class RenderPipeline
	{
		friend class Application;
		protected:
			struct LWGC_PerFrame
			{
				glm::vec4	time;
			};

			std::vector< VkSemaphore >		imageAvailableSemaphores;
			std::vector< VkSemaphore >		renderFinishedSemaphores;
			std::vector< VkFence >			inFlightFences;
			VkDevice						device;
			VulkanInstance *				instance;
			size_t							currentFrame = 0;
			RenderPass						renderPass;
			VkFramebuffer					framebuffer;
			SwapChain *						swapChain;
			std::vector< VkCommandBuffer >	frameCommandBuffers;
			CommandBufferPool *				mainCommandPool;
			bool							framebufferResized;
			Camera *						currentCamera;
			DescriptorSet					perFrameSet;

			UniformBuffer					_uniformPerFrame;

			void				SetLastRenderPass(const RenderPass & renderPass);
			VkCommandBuffer		GetCurrentFrameCommandBuffer(void);
			VkFramebuffer		GetCurrentFrameBuffer(void);

			virtual void		CreateRenderPass(void);
			virtual void		RecreateSwapChain(void);
			virtual void		Render(const std::vector< Camera * > & cameras, RenderContext * context) = 0;
			virtual void		InitializeHandles(void) noexcept;
			virtual void		PresentFrame(void);
			virtual void		Initialize(SwapChain * swapChain);
			virtual void		CreateSyncObjects(void);
			virtual void		RenderGUI(RenderContext * context) noexcept;

			// API to record command on predefined object lists
			void				RecordAllComputeDispatches(RenderPass & pass, RenderContext * context);
			void				RecordAllMeshRenderers(RenderPass & pass, RenderContext * context);

		// The private part is only used as internal render-pipeline setup and should be overwritten by a custom render pipeline
		private:
			LWGC_PerFrame					_perFrame;
			uint32_t						_imageIndex;
			bool							_initialized;

			bool				RenderInternal(const std::vector< Camera * > & cameras, RenderContext * context);

			void				UpdatePerframeUnformBuffer(void) noexcept;

		public:
			RenderPipeline(void);
			RenderPipeline(const RenderPipeline & p) = delete;
			virtual			~RenderPipeline(void);

			RenderPipeline & operator=(const RenderPipeline & rhs) = delete;

			RenderPass *	GetRenderPass(void);
			Camera *		GetCurrentCamera(void);
			bool			IsInitialized(void);

			void			EnqueueFrameCommandBuffer(VkCommandBuffer cmd);

			static RenderPipeline *	Get();
	};
}
