#include "RenderPass.hpp"

#include "Core/Vulkan/Material.hpp"

using namespace LWGC;

RenderPass::RenderPass(void)
{
	this->_renderPass = VK_NULL_HANDLE;
	this->_attachmentCount = 0;
}

RenderPass::~RenderPass(void)
{
	std::cout << "Destroying renderpass\n";
	Cleanup();
}

void		RenderPass::Initialize(SwapChain * swapChain) noexcept
{
	_instance = VulkanInstance::Get();
	_swapChain = swapChain;
}

void		RenderPass::AddAttachment(const VkAttachmentDescription & attachment, VkImageLayout layout) noexcept
{
	_references.push_back({_attachmentCount++, layout});
	_attachments.push_back(attachment);
}

void		RenderPass::SetDepthAttachment(const VkAttachmentDescription & attachment, VkImageLayout layout) noexcept
{
	_depthAttachmentRef = {_attachmentCount++, layout};
	_attachments.push_back(attachment);
}

void		RenderPass::Cleanup(void) noexcept
{
	if (_renderPass != VK_NULL_HANDLE)
		vkDestroyRenderPass(_instance->GetDevice(), _renderPass, nullptr);

	_attachmentCount = 0;
	_attachments.clear();
	_references.clear();
	_dependencies.clear();
}

void		RenderPass::AddDependency(const VkSubpassDependency & dependency) noexcept
{
	_dependencies.push_back(dependency);
}

void		RenderPass::Create(void)
{
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = _references.size();
	subpass.pColorAttachments = _references.data();
	subpass.pDepthStencilAttachment = &_depthAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(_attachments.size());
	renderPassInfo.pAttachments = _attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = _dependencies.size();
	renderPassInfo.pDependencies = _dependencies.data();

	auto device = _instance->GetDevice();
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");

	std::cout << "Created subpass !\n";
	_swapChain->CreateFrameBuffers(*this);
}

bool	RenderPass::BindDescriptorSet(const std::string & name, VkDescriptorSet set)
{
	DescriptorBindings::iterator binding = _currentBindings.find(name);
	if (binding == _currentBindings.end())
	{
		_currentBindings[name] = {set, true};
	}
	else
	{
		if (binding->second.set != set)
		{
			binding->second.set = set;
			binding->second.hasChanged = true;
		}
	}
	return true;
}

void	RenderPass::BindMaterial(Material * material)
{
	_currentMaterial = material;

	material->BindDescriptorSets(this);

	// mark all bindings to changed set they're all rebinded to the new material
	for (auto & b : _currentBindings)
		b.second.hasChanged = true;
}

void	RenderPass::BeginFrame(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer)
{
	_commandBuffer = commandBuffer;
	_framebuffer = framebuffer;
}

void	RenderPass::UpdateDescriptorBindings(VkCommandBuffer cmd)
{
	// Bind all descriptor that have changed
	for (auto & b : _currentBindings)
	{
		if (b.second.hasChanged)
		{
			uint32_t firstSet = _currentMaterial->GetDescriptorSetBinding(b.first);

			if (firstSet != -1u)
			{
				vkCmdBindDescriptorSets(
					cmd,
					_currentMaterial->IsCompute() ?	VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
					_currentMaterial->GetPipelineLayout(),
					firstSet,
					1, &b.second.set,
					0, nullptr
				);
			}
		}
	}
}

void	RenderPass::BeginSecondaryCommandBuffer(VkCommandBuffer cmd, VkCommandBufferUsageFlagBits commandBufferUsage)
{
	VkCommandBufferInheritanceInfo	inheritanceInfo = {};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.renderPass = _renderPass;
	inheritanceInfo.framebuffer = _framebuffer;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = commandBufferUsage;
	beginInfo.pInheritanceInfo = &inheritanceInfo;

	if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	// The bind pipelines should be sorted to avoid unnecessary pipeline switch
	vkCmdBindPipeline(
		cmd,
		_currentMaterial->IsCompute() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
		_currentMaterial->GetPipeline()
	);
	
	UpdateDescriptorBindings(cmd);
}

void	RenderPass::ExecuteCommandBuffer(VkCommandBuffer cmd)
{
	if (vkEndCommandBuffer(cmd) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}

	vkCmdExecuteCommands(_commandBuffer, 1, &cmd);
}

void	RenderPass::ClearBindings(void)
{
	_currentBindings.clear();
}

VkAttachmentDescription RenderPass::GetDefaultColorAttachment(VkFormat format) noexcept
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	return colorAttachment;
}

VkAttachmentDescription RenderPass::GetDefaultDepthAttachment(VkFormat format) noexcept
{
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = format;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	return depthAttachment;
}

VkRenderPass	RenderPass::GetRenderPass(void) const noexcept { return (this->_renderPass); }

std::ostream &	operator<<(std::ostream & o, RenderPass const & r)
{
	o << "RenderPass" << std::endl;
	(void)r;
	return (o);
}
