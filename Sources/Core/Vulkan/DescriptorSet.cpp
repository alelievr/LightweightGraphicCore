#include "DescriptorSet.hpp"

using namespace LWGC;

DescriptorSet::DescriptorSet(void) :
	_descriptorSetLayout(VK_NULL_HANDLE), _descriptorSet(VK_NULL_HANDLE),
	_created(false), _stageFlags(VK_SHADER_STAGE_ALL)
{

}

DescriptorSet::~DescriptorSet(void)
{
}

void					DescriptorSet::SetStage(VkShaderStageFlagBits stageFlags)
{
	_stageFlags = stageFlags;
}

void					DescriptorSet::AddBinding(uint32_t index, Texture * texture, VkDescriptorType descriptorType)
{
	_layoutBinding.push_back(Vk::CreateDescriptorSetLayoutBinding(index, descriptorType, _stageFlags));

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = texture->GetLayout();
	imageInfo.imageView = texture->GetView();
	imageInfo.sampler = 0;

	_bindingInfos[index] = BindingInfo{
		imageInfo,
		{},
		{},
		descriptorType
	};
}

void					DescriptorSet::AddBinding(uint32_t index, VkDescriptorType descriptorType, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset)
{
	_layoutBinding.push_back(Vk::CreateDescriptorSetLayoutBinding(index, descriptorType, _stageFlags));

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = range;

	_bindingInfos[index] = BindingInfo{
		{},
		bufferInfo,
		{},
		descriptorType
	};
}

void					DescriptorSet::CreateBindings(void)
{
	VkDevice device = VulkanInstance::Get()->GetDevice();

	// TODO: pool the descriptor set layouts so we can reuse them if we have multiple DescriptroSet of the same type
	Vk::CreateDescriptorSetLayout(_layoutBinding, _descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = VulkanInstance::Get()->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1u;
	allocInfo.pSetLayouts = &_descriptorSetLayout;

	if (vkAllocateDescriptorSets(device, &allocInfo, &_descriptorSet) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	std::vector< VkWriteDescriptorSet > descriptorWrites;
	for (const auto & bindingInfo : _bindingInfos)
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = _descriptorSet;
		descriptorWrite.dstBinding = bindingInfo.first;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = bindingInfo.second.descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bindingInfo.second.bufferInfo;
		descriptorWrite.pImageInfo = &bindingInfo.second.imageInfo;
		descriptorWrite.pTexelBufferView = &bindingInfo.second.viewInfo;

		descriptorWrites.push_back(descriptorWrite);
	}

	vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

	_created = true;
}

VkDescriptorSetLayout	DescriptorSet::GetDescriptorSetLayout(void)
{
	if (!_created)
		CreateBindings();

	return this->_descriptorSetLayout;
}

VkDescriptorSet			DescriptorSet::GetDescriptorSet(void)
{
	if (!_created)
		CreateBindings();

	return this->_descriptorSet;
}

std::ostream &	operator<<(std::ostream & o, DescriptorSet const & r)
{
	o << "DescriptorSet" << std::endl;
	(void)r;
	return (o);
}
