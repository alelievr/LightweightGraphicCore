#include "ShaderBindingTable.hpp"

#include "Core/Vulkan/Vk.hpp"
#include <cmath>
#include <algorithm>

using namespace LWGC;

ShaderBindingTable::ShaderBindingTable(void)
{
}

ShaderBindingTable::~ShaderBindingTable(void)
{
	VkDevice	device = VulkanInstance::Get()->GetDevice();

	for (const auto & layouts : _elementLayouts)
	{
		vkDestroyDescriptorSetLayout(device, layouts.second, nullptr);
	}
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

PushConstantBinding &	ShaderBindingTable::AddPushConstant(const std::string & name, uint32_t offset, uint32_t size)
{
	return (_pushConstants.insert({name, PushConstantBinding{
		offset,
		size
	}}).first)->second;
}

void					ShaderBindingTable::GenerateSetLayouts()
{
	std::unordered_map< int, std::vector< VkDescriptorSetLayoutBinding > >	layoutBindings;
	std::unordered_map< int, std::vector< int > >							layoutBindingIndices;
	int		maxDescriptorSet = 0;

	for (const auto & binding : _bindings)
	{
		auto & bind = binding.second;
		auto b = Vk::CreateDescriptorSetLayoutBinding(bind.bindingIndex, bind.descriptorType, _stageFlags);
		layoutBindings[bind.descriptorSet].push_back(b);
		layoutBindingIndices[bind.descriptorSet].push_back(bind.bindingIndex);
		maxDescriptorSet = fmax(maxDescriptorSet, bind.descriptorSet);
	}

	// We add 1 to the max descriptor set because we want to include the last index
	maxDescriptorSet += 1;

	_descriptorSetLayout.resize(maxDescriptorSet);
	for (int i = 0; i < maxDescriptorSet; i++)
	{
		// Fill gaps with empty descriptor sets so they're all contiguous
		if (layoutBindings.find(i) == layoutBindings.end())
		{
			std::cout << "Empty descriptor set at: " << i << "\n";
			Vk::CreateDescriptorSetLayout({}, _descriptorSetLayout[i]);
		}
		else
		{
			Vk::CreateDescriptorSetLayout(layoutBindings[i], _descriptorSetLayout[i]);
			for (const auto & binding : _bindings)
			{
				if (binding.second.descriptorSet == i)
					_elementLayouts[binding.first] = _descriptorSetLayout[i];
			}

			// Check for descriptor set index gaps
			auto & indices = layoutBindingIndices[i];
			int max = *std::max_element(indices.begin(), indices.end());
			for (int j = 0; j < max && j < 10; j++)
			{
				// TODO: only report this error when the property is set from the material
				if (std::find(indices.begin(), indices.end(), j) == indices.end())
					std::cerr << "Error in shader descriptor set layout #" << i << ", Binding #" << j << " does not exists (but another binding with a greater index exists)" << std::endl;
			}
		}
	}
}

VkDescriptorSetLayout	ShaderBindingTable::GetDescriptorSetLayout(const std::string & setElementName) const
{
	return _elementLayouts.find(setElementName)->second;
}

bool					ShaderBindingTable::HasBinding(const std::string & bindingName) const
{
	return _bindings.find(bindingName) != _bindings.end();
}

uint32_t				ShaderBindingTable::GetDescriptorSetBinding(const std::string & bindingName) const
{
	return _bindings.find(bindingName)->second.descriptorSet;
}

uint32_t				ShaderBindingTable::GetDescriptorIndex(const std::string & bindingName) const
{
	return _bindings.find(bindingName)->second.bindingIndex;
}

std::vector< std::string >	ShaderBindingTable::GetBindingNames(void) const
{
	std::vector< std::string > bindings;

	for (const auto & k : _bindings)
		bindings.push_back(k.first);

	return bindings;
}

const std::vector< VkDescriptorSetLayout > &	ShaderBindingTable::GetDescriptorSetLayouts(void) const { return _descriptorSetLayout; }

const std::vector< VkPushConstantRange >		ShaderBindingTable::GetPushConstants(VkShaderStageFlags stages) const
{
	std::vector< VkPushConstantRange > pushConstantRanges;

	uint32_t size = 0;
	for (const auto & pushConstantKP : _pushConstants)
		size += pushConstantKP.second.size;

	// Currently we support only one block if push constant access because the VkPushConstantRange
	// describe the memory zones that will be accessible by stage flags, we combine all our push constants
	// into one big range.
	if (size > 0)
	{
		pushConstantRanges.push_back(VkPushConstantRange{
			stages,
			0,
			size,
		});
	}

	return pushConstantRanges;
}

const PushConstantBinding	ShaderBindingTable::GetPushConstant(const std::string & name) const
{
	return _pushConstants.find(name)->second;
}

bool						ShaderBindingTable::HasPushConstant(const std::string & name) const
{
	return _pushConstants.find(name) != _pushConstants.end();
}

std::ostream &	operator<<(std::ostream & o, ShaderBindingTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
