#pragma once

#include <iostream>
#include <string>

#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/RenderPass.hpp"
#include "Core/Components/ComputeDispatcher.hpp"

#include "IncludeDeps.hpp"
#include GLM_INCLUDE
#include VULKAN_INCLUDE

namespace LWGC
{
	class		ComputeShader
	{
		private:
			Material *			_material;
			ComputeDispatcher	_dispatcher;
			RenderPass			_renderPass;

		public:
			ComputeShader(void);
			ComputeShader(const std::string & shaderPath);
			ComputeShader(const ComputeShader&) = delete;
			virtual ~ComputeShader(void) = default;

			ComputeShader &	operator=(ComputeShader const & src) = delete;

			void	LoadShader(const std::string & shaderPath);
			void	Dispatch(VkCommandBuffer cmd, int width, int height, int depth) noexcept;

			void				SetBuffer(const std::string & bindingName, VkBuffer buffer, size_t size, VkDescriptorType descriptorType, bool silent = false);
			void				SetTexture(const std::string & bindingName, const Texture * texture, VkImageLayout imageLayout, VkDescriptorType descriptorType, bool silent = false);
			void				SetSampler(const std::string & bindingName, VkSampler sampler, bool silent = false);
	};

	std::ostream &	operator<<(std::ostream & o, ComputeShader const & r);
}