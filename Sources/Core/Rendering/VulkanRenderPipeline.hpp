#pragma once

#include <array>

#include "RenderTarget.hpp"
#include "Core/Components/Camera.hpp"
#include "RenderContext.hpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"
#include "Core/Mesh.hpp"
#include "Core/Handles/Selection.hpp"

#include IMGUI_INCLUDE

#define MAX_FRAMES_IN_FLIGHT	2
#define PER_FRAME_BINDING_INDEX	0

namespace LWGC
{
	class VulkanRenderPipeline
	{
		friend class Application;

		// The private part is only used as internal render-pipeline setup and should be overwritten by a custom render pipeline
		private:
			struct LWGC_PerFrame
			{
				glm::vec4	time;
			};

			static VulkanRenderPipeline *	_pipelineInstance;
			std::vector< VkCommandBuffer >	_swapChainCommandBuffers;
			LWGC_PerFrame					_perFrame;
			UniformBuffer					_uniformPerFrame;
			VkDescriptorSet					_perFrameDescriptorSet;
			VkDescriptorSetLayout			_perFrameDescriptorSetLayout;

			void				RenderInternal(const std::vector< Camera * > & cameras, RenderContext & context);
			void				UpdatePerframeUnformBuffer(void) noexcept;
			void				CreatePerFrameDescriptorSet(void) noexcept;

		protected:
			std::vector<VkSemaphore>		imageAvailableSemaphores;
			std::vector<VkSemaphore>		renderFinishedSemaphores;
			std::vector<VkFence>			inFlightFences;
			VkDevice						device;
			VulkanInstance *				instance;
			size_t							currentFrame = 0;
			RenderPass						renderPass;
			VkFramebuffer					framebuffer;
			SwapChain *						swapChain;
			VkCommandBuffer					commandBuffer;
			bool							framebufferResized;
			Selection						selection;

			virtual void		CreateRenderPass(void);
			void				BeginRenderPass(RenderContext & context);
			void				EndRenderPass(void);
			virtual void		RecreateSwapChain(RenderContext & renderContext);
			virtual void		Render(const std::vector< Camera * > & cameras, RenderContext & context) = 0;
			virtual void		CreateDescriptorSets(void);

		public:
			VulkanRenderPipeline(void);
			VulkanRenderPipeline(const VulkanRenderPipeline & p) = delete;
			virtual			~VulkanRenderPipeline(void);

			VulkanRenderPipeline & operator=(const VulkanRenderPipeline & rhs) = delete;

			virtual void	Initialize(SwapChain * swapChain);
			virtual void	CreateSyncObjects(void);
			virtual void	RenderGUI(void) noexcept;

			SwapChain *		GetSwapChain(void);
			RenderPass *	GetRenderPass(void);

			static VulkanRenderPipeline *	Get();
	};
}
