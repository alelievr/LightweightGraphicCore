#include "DescriptorSet.hpp"

using namespace LWGC;

DescriptorSet::DescriptorSet(VkShaderStageFlagBits stageFlags) :
	_descriptorSetLayout(VK_NULL_HANDLE), _descriptorSet(VK_NULL_HANDLE),
	_created(false), _stageFlags(stageFlags)
{
}

DescriptorSet::~DescriptorSet(void)
{
	std::cout << "Destructor of DescriptorSet called" << std::endl;
}

void					DescriptorSet::AddBinding(uint32_t index, const Texture & texture, VkDescriptorType descriptorType)
{
	_layoutBinding.push_back(Vk::CreateDescriptorSetLayoutBinding(index, descriptorType, _stageFlags));
	_bindingInfos[index] = BindingInfo{

	};
}

void					DescriptorSet::CreateBindings(void)
{
	VkDevice device = VulkanInstance::Get()->GetDevice();

	// TODO: pool the descriptor set layouts so we can reuse them if we have multiple DescriptroSet of the same type
	Vk::CreateDescriptorSetLayout(_layoutBinding, _descriptorSetLayout);

	// TODO: allocate the descriptor set and upload all the datas
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

	}
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = descriptorType;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
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
