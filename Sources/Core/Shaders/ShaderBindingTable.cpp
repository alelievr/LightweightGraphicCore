#include "ShaderBindingTable.hpp"

#include "Core/Vulkan/Vk.hpp"
#include <cmath>

using namespace LWGC;

ShaderBindingTable::ShaderBindingTable(void)
{
	std::cout << "Default constructor of ShaderBindingTable called" << std::endl;
}

ShaderBindingTable::~ShaderBindingTable(void)
{
	std::cout << "Destructor of ShaderBindingTable called" << std::endl;
}

void			ShaderBindingTable::SetStage(VkShaderStageFlagBits stage)
{
	_stageFlags = stage;
}

ShaderBinding &	ShaderBindingTable::AddBinding(const std::string & name, int descriptorSet, int bindingIndex, VkDescriptorType descriptorType)
{
	return (*(_bindings.insert({name, ShaderBinding{
		descriptorSet,
		bindingIndex,
		descriptorType,
		0
	}}).first)).second;
}

void			ShaderBindingTable::GenerateSetLayouts()
{
	std::unordered_map< int, std::vector< VkDescriptorSetLayoutBinding > >	layoutBindings;
	int		maxDescriptorSet = 0;
	
	for (const auto & binding : _bindings)
	{
		auto b = Vk::CreateDescriptorSetLayoutBinding(binding.second.bindingIndex, binding.second.descriptorType, _stageFlags);
		layoutBindings[binding.second.descriptorSet].push_back(b);
		maxDescriptorSet = fmax(maxDescriptorSet, binding.second.descriptorSet);
	}

	_descriptorSetLayout.resize(maxDescriptorSet);
	for (int i = 0; i < maxDescriptorSet; i++)
	{
		// Fill empty descriptor sets with so they're all contiguous
		if (layoutBindings.find(i) == layoutBindings.end())
			Vk::CreateDescriptorSetLayout({}, _descriptorSetLayout[i]);
		else
			Vk::CreateDescriptorSetLayout(layoutBindings[i], _descriptorSetLayout[i]);
	}
}

uint32_t		ShaderBindingTable::GetDescriptorSetBinding(const std::string & bindingName)
{
	return _bindings[bindingName].descriptorSet;
}

const std::vector< VkDescriptorSetLayout > &	ShaderBindingTable::GetDescriptorSetLayout(void) const { return _descriptorSetLayout; }

std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
