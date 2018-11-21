#pragma once

#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE

struct		UniformBuffer
{
	VkBuffer		buffer;
	VkDeviceMemory	memory;
};
