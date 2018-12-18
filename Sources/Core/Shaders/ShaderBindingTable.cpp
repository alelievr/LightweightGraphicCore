#include "ShaderBindingTable.hpp"

#include "Core/Vulkan/Vk.hpp"
#include <cmath>

using namespace LWGC;

ShaderBindingTable::ShaderBindingTable(void)
{
}

ShaderBindingTable::~ShaderBindingTable(void)
{
}

void					ShaderBindingTable::SetStage(VkShaderStageFlagBits stage)
{
	_stageFlags = stage;
}

ShaderBinding &			ShaderBindingTable::AddBinding(const std::string & name, int descriptorSet, int bindingIndex, VkDescriptorType descriptorType)
{
	// Check for same-location bindings:

	for (const auto & binding : _bindings)
	{
		if (binding.second.bindingIndex == bindingIndex && binding.second.descriptorSet == descriptorSet && binding.first != name)
			std::cerr << "WARNING: resource " << name << " and " << binding.first << " are bound to the same location: " << bindingIndex << ":" << descriptorSet << std::endl;
	}

	return (_bindings.insert({name, ShaderBinding{
		descriptorSet,
		bindingIndex,
		descriptorType,
		0
	}}).first)->second;
}

void					ShaderBindingTable::GenerateSetLayouts()
{
	std::unordered_map< int, std::vector< VkDescriptorSetLayoutBinding > >	layoutBindings;
	int		maxDescriptorSet = 0;
	
	for (const auto & binding : _bindings)
	{
		auto b = Vk::CreateDescriptorSetLayoutBinding(binding.second.bindingIndex, binding.second.descriptorType, _stageFlags);
		layoutBindings[binding.second.descriptorSet].push_back(b);
		maxDescriptorSet = fmax(maxDescriptorSet, binding.second.descriptorSet);
	}

	// We add 1 to the max descriptor set because we want to include the last index
	maxDescriptorSet += 1;

	_descriptorSetLayout.resize(maxDescriptorSet);
	for (int i = 0; i < maxDescriptorSet; i++)
	{
		// Fill gaps with empty descriptor sets so they're all contiguous
		if (layoutBindings.find(i) == layoutBindings.end())
			Vk::CreateDescriptorSetLayout({}, _descriptorSetLayout[i]);
		else
		{
			Vk::CreateDescriptorSetLayout(layoutBindings[i], _descriptorSetLayout[i]);
			for (const auto & binding : _bindings)
			{
				if (binding.second.descriptorSet == i)
					_elementLayouts[binding.first] = _descriptorSetLayout[i];
			}
		}
	}
}

VkDescriptorSetLayout	ShaderBindingTable::GetDescriptorSetLayout(const std::string & setElementName) const
{
	auto layout = _elementLayouts.find(setElementName);

	if (layout == _elementLayouts.end())
		throw std::runtime_error("Can't find descriptor set layout with a field named '" + setElementName + "'");

	return layout->second;
}

bool					ShaderBindingTable::HasBinding(const std::string & bindingName) const
{
	return _bindings.find(bindingName) != _bindings.end();
}

uint32_t				ShaderBindingTable::GetDescriptorSetBinding(const std::string & bindingName) const
{
	auto set = _bindings.find(bindingName);

	if (set == _bindings.end())
		throw std::runtime_error("Can't find descriptor set for binding name: '" + bindingName + "'");
	
	return set->second.descriptorSet;
}

uint32_t				ShaderBindingTable::GetDescriptorIndex(const std::string & bindingName) const
{
	auto set = _bindings.find(bindingName);

	if (set == _bindings.end())
		throw std::runtime_error("Can't find descriptor index for binding name: '" + bindingName + "'");
	
	return set->second.bindingIndex;
}

std::vector< std::string >	ShaderBindingTable::GetBindingNames()
{
	std::vector< std::string > bindings;

	for (const auto & k : _bindings)
		bindings.push_back(k.first);

	return bindings;
}

const std::vector< VkDescriptorSetLayout > &	ShaderBindingTable::GetDescriptorSetLayouts(void) const { return _descriptorSetLayout; }

std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
