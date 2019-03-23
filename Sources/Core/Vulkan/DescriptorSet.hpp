#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Core/Vulkan/UniformBuffer.hpp"
#include "Core/Textures/Texture.hpp"

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE

namespace LWGC
{
	class		DescriptorSet
	{
		private:
			struct BindingInfo
			{
				VkDescriptorImageInfo	imageInfo;
				VkDescriptorBufferInfo	bufferInfo;
				VkBufferView			viewInfo;
			};

			UniformBuffer			_buffer;
			VkDescriptorSetLayout	_descriptorSetLayout;
			VkDescriptorSet			_descriptorSet;
			std::vector< VkDescriptorSetLayoutBinding >	_layoutBinding;
			std::unordered_map< uint32_t, BindingInfo > _bindingInfos;
			bool					_created;
			VkShaderStageFlagBits	_stageFlags;

			void					CreateBindings(void);

		public:
			DescriptorSet(void) = delete;
			DescriptorSet(VkShaderStageFlagBits stageFlags);
			DescriptorSet(const DescriptorSet &) = delete;
			virtual ~DescriptorSet(void);

			DescriptorSet &	operator=(DescriptorSet const & src) = delete;

			void					AddBinding(uint32_t index, const Texture & texture, VkDescriptorType descriptorType);
			// TODO: bindings for buffers and samplers

			VkDescriptorSetLayout	GetDescriptorSetLayout(void);
			VkDescriptorSet			GetDescriptorSet(void);
	};

	std::ostream &	operator<<(std::ostream & o, DescriptorSet const & r);
}
