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
				VkDescriptorType		descriptorType;
			};

			VkDescriptorSetLayout	_descriptorSetLayout;
			VkDescriptorSet			_descriptorSet;
			std::vector< VkDescriptorSetLayoutBinding >	_layoutBinding;
			std::unordered_map< uint32_t, BindingInfo > _bindingInfos;
			bool					_created;
			VkShaderStageFlagBits	_stageFlags;

			void					CreateBindings(void);

		public:
			DescriptorSet(void);
			DescriptorSet(const DescriptorSet &) = delete;
			virtual ~DescriptorSet(void);

			DescriptorSet &	operator=(DescriptorSet const & src) = delete;

			void					SetStage(VkShaderStageFlagBits stageFlags);

			void					AddBinding(uint32_t index, Texture * texture, VkDescriptorType descriptorType);
			void					AddBinding(uint32_t index, VkDescriptorType descriptorType, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset = 0);
			// TODO: bindings for buffers, samplers and texel buffers

			// TODO: function to change the values within the descriptor sets

			VkDescriptorSetLayout	GetDescriptorSetLayout(void);
			VkDescriptorSet			GetDescriptorSet(void);
	};

	std::ostream &	operator<<(std::ostream & o, DescriptorSet const & r);
}
