#include "RenderPass.hpp"

#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

RenderPass::RenderPass(void) : _instance(nullptr), _currentMaterial(nullptr)
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
	_swapChain = swapChain;
	_instance = VulkanInstance::Get();
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

void		RenderPass::Create(bool computeOnly)
{
	if (computeOnly)
	{
		std::cerr << "For compute we don't need to call Create on renderpass because we don't need one to run a compute shader" << std::endl;
		return ;
	}

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

bool	RenderPass::BindDescriptorSet(const std::string & name, DescriptorSet & set)
{
	return BindDescriptorSet(name, set.GetDescriptorSet());
}

void	RenderPass::BindMaterial(Material * material)
{
	_currentMaterial = material;

	// mark all bindings to changed set they're all rebinded to the new material
	// TODO: only do this when the pipeline layout changes, if only the shader changes we just have to update
	// the descriptor that are not at the same index / new ones
	for (auto & b : _currentBindings)
		b.second.hasChanged = true;
}

void	RenderPass::Begin(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, const std::string & passName)
{
	_commandBuffer = commandBuffer;
	_framebuffer = framebuffer;

	if (VulkanInstance::AreDebugMarkersEnabled())
	{
		Vk::BeginProfilingSample(_commandBuffer, passName, Color::Cyan);
	}

	// If there is no framebuffer to bind, it means we're in a compute shader pass
	if (framebuffer != VK_NULL_HANDLE)
	{
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = _swapChain->GetExtent();
		renderPassInfo.clearValueCount = _clearValues.size();
		renderPassInfo.pClearValues = _clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
}

void	RenderPass::End(void)
{
	if (VulkanInstance::AreDebugMarkersEnabled())
	{
		Vk::EndProfilingSample(_commandBuffer);
	}

	if (_framebuffer != VK_NULL_HANDLE)
	{
		vkCmdEndRenderPass(_commandBuffer);
	}
}

void	RenderPass::UpdateDescriptorBindings(void)
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
					_commandBuffer,
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

void	RenderPass::ClearBindings(void)
{
	_currentBindings.clear();
}

void	RenderPass::SetClearColor(const Color & color, float depth, uint32_t stencil)
{
	_clearValues.resize(2);
	_clearValues[0].color = {{color.r, color.g, color.b, color.a}};
	_clearValues[1].depthStencil = {depth, stencil};
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
VkCommandBuffer	RenderPass::GetCommandBuffer(void) const noexcept { return (this->_commandBuffer); }

std::ostream &	operator<<(std::ostream & o, RenderPass const & r)
{
	o << "RenderPass" << std::endl;
	(void)r;
	return (o);
}
