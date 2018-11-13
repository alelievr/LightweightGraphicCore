#pragma once

#include "RenderTarget.hpp"
#include "RenderQueueType.hpp"
#include "IRenderable.hpp"
#include "Camera.hpp"
#include "RenderContext.hpp"
#include "Vulkan/RenderPass.hpp"
#include "Vulkan/Material.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/VulkanInstance.hpp"

namespace LWGC
{
	class VulkanRenderPipeline
	{
		private:
			// TODO: move material into the MeshRenderer
			Material			material;

		protected:
			RenderPass			renderPass;
			SwapChain *			swapChain;
			VulkanInstance *	instance;
			CommandBufferPool *	graphicCommandBufferPool;

		public:
			VulkanRenderPipeline(void);
			VulkanRenderPipeline(const VulkanRenderPipeline & p) = delete;
			virtual		~VulkanRenderPipeline(void);

			VulkanRenderPipeline & operator=(const VulkanRenderPipeline & rhs) = delete;

			virtual void	Initialize(SwapChain * swapChain);
			virtual void	Render(const std::vector< Camera > & cameras, const RenderContext & context) = 0;
			virtual void	CreateMaterials(void);
			virtual void	CreateRenderPass(void);
	};
}
