#pragma once

#include <array>

#include "RenderTarget.hpp"
#include "Core/Components/Camera.hpp"
#include "RenderContext.tpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Mesh.hpp"

#include IMGUI_INCLUDE

#define PER_FRAME_BINDING_INDEX	0

namespace LWGC
{
	using BeginFrameRenderingDelegate = Delegate< void(void) >;
	using BeginCameraRenderingDelegate = Delegate< void(Camera *) >;
	using EndCameraRenderingDelegate = Delegate< void(Camera *) >;
	using EndFrameRenderingDelegate = Delegate< void(void) >;

	class RenderPipeline
	{
		friend class Application;

		// The private part is only used as internal render-pipeline setup and should be overwritten by a custom render pipeline
		private:
			struct LWGC_PerFrame
			{
				glm::vec4	time;
			};

			static RenderPipeline *	_pipelineInstance;
			std::vector< VkCommandBuffer >	_swapChainCommandBuffers;
			LWGC_PerFrame					_perFrame;
			UniformBuffer					_uniformPerFrame;
			VkDescriptorSet					_perFrameDescriptorSet;
			VkDescriptorSetLayout			_perFrameDescriptorSetLayout;
			uint32_t						_imageIndex;

			void				RenderInternal(const std::vector< Camera * > & cameras, RenderContext * context);
			void				UpdatePerframeUnformBuffer(void) noexcept;
			void				CreatePerFrameDescriptorSet(void);

		protected:
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
			bool							framebufferResized;
			Camera *						currentCamera;

			virtual void		CreateRenderPass(void);
			void				BeginRenderPass(RenderContext * context);
			void				EndRenderPass(void);
			virtual void		RecreateSwapChain(void);
			virtual void		Render(const std::vector< Camera * > & cameras, RenderContext * context) = 0;
			virtual void		CreateDescriptorSets(void);
			virtual void		InitializeHandles(void) noexcept;
			virtual void		PresentFrame(void);
			virtual void		Initialize(SwapChain * swapChain);
			virtual void		CreateSyncObjects(void);
			virtual void		RenderGUI(RenderContext * context) noexcept;

		public:
			RenderPipeline(void);
			RenderPipeline(const RenderPipeline & p) = delete;
			virtual			~RenderPipeline(void);

			RenderPipeline & operator=(const RenderPipeline & rhs) = delete;

			SwapChain *		GetSwapChain(void);
			RenderPass *	GetRenderPass(void);
			Camera *		GetCurrentCamera(void);
			void			EnqueueFrameCommandBuffer(VkCommandBuffer cmd);

			BeginFrameRenderingDelegate		beginFrameRendering;
			BeginCameraRenderingDelegate	beginCameraRendering;
			EndCameraRenderingDelegate		endCameraRendering;
			EndFrameRenderingDelegate		endFrameRendering;

			static RenderPipeline *	Get();
	};
}
