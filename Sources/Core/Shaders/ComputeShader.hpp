#pragma once

#include <iostream>
#include <string>

#include "Core/Vulkan/Material.hpp"
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

		public:
			ComputeShader(void);
			ComputeShader(const std::string & shaderPath);
			ComputeShader(const ComputeShader&) = delete;
			virtual ~ComputeShader(void) = default;

			ComputeShader &	operator=(ComputeShader const & src) = delete;

			void	LoadShader(const std::string & shaderPath);
			void	Dispatch(VkCommandBuffer cmd, glm::ivec3 dispatchSize);
	};

	std::ostream &	operator<<(std::ostream & o, ComputeShader const & r);
}