#pragma once

#include <vector>

#include "IncludeDeps.hpp"
#include VULKAN_INCLUDE

namespace LWGC
{
	class MaterialState
	{
		public:
			inline constexpr static VkPipelineDepthStencilStateCreateInfo CreateDepthState(bool depthTest, bool depthWrite, VkCompareOp compareOp = VK_COMPARE_OP_LESS)
			{
				VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
				depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencilState.depthTestEnable = depthTest;
				depthStencilState.depthWriteEnable = depthWrite;
				depthStencilState.depthCompareOp = compareOp;
				depthStencilState.depthBoundsTestEnable = VK_FALSE;
				depthStencilState.minDepthBounds = 0.0f;
				depthStencilState.maxDepthBounds = 1.0f;
				depthStencilState.stencilTestEnable = VK_FALSE;
				depthStencilState.front = {};
				depthStencilState.back = {};

				return depthStencilState;
			}

			inline constexpr static VkPipelineDepthStencilStateCreateInfo CreateDepthState(bool depthTest, bool depthWrite, VkStencilOpState front, VkStencilOpState back, VkCompareOp compareOp = VK_COMPARE_OP_LESS)
			{
				VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
				depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				depthStencilState.depthTestEnable = depthTest;
				depthStencilState.depthWriteEnable = depthWrite;
				depthStencilState.depthCompareOp = compareOp;
				depthStencilState.depthBoundsTestEnable = VK_FALSE;
				depthStencilState.minDepthBounds = 0.0f;
				depthStencilState.maxDepthBounds = 1.0f;
				depthStencilState.stencilTestEnable = VK_TRUE;
				depthStencilState.front = front;
				depthStencilState.back = back;

				return depthStencilState;
			}

			inline constexpr static VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState(
				bool blend,
				VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
				VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VkBlendOp colorBlendOp = VK_BLEND_OP_ADD,
				VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
				VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
				VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD
			)
			{
				VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
				colorBlendAttachment.colorWriteMask = colorWriteMask;
				colorBlendAttachment.blendEnable = blend;
				colorBlendAttachment.srcColorBlendFactor = srcColorBlendFactor;
				colorBlendAttachment.dstColorBlendFactor = dstColorBlendFactor;
				colorBlendAttachment.colorBlendOp = colorBlendOp;
				colorBlendAttachment.srcAlphaBlendFactor = srcAlphaBlendFactor;
				colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor;
				colorBlendAttachment.alphaBlendOp = alphaBlendOp;

				return colorBlendAttachment;
			}
			inline constexpr static VkPipelineColorBlendStateCreateInfo CreateColorBlendState(uint32_t blendAttachmentCount = 0, const std::vector< VkPipelineColorBlendAttachmentState > & blendAttachments = {})
			{
				VkPipelineColorBlendStateCreateInfo colorBlendState = {};
				colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				colorBlendState.logicOpEnable = VK_FALSE;
				colorBlendState.logicOp = VK_LOGIC_OP_COPY;
				colorBlendState.attachmentCount = blendAttachmentCount;
				colorBlendState.pAttachments = blendAttachments.data();
				colorBlendState.blendConstants[0] = 0.0f;
				colorBlendState.blendConstants[1] = 0.0f;
				colorBlendState.blendConstants[2] = 0.0f;
				colorBlendState.blendConstants[3] = 0.0f;

				return colorBlendState;
			}

			inline constexpr static VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyState(VkPrimitiveTopology topology, bool primiviteRestart = false)
			{
				VkPipelineInputAssemblyStateCreateInfo	inputAssemblyState = {};

				inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				inputAssemblyState.topology = topology;
				inputAssemblyState.primitiveRestartEnable = primiviteRestart;

				return inputAssemblyState;
			}

			// TODO: function to create rasterization state and vertex input state ?

			// Color blend attachment states
			inline static const VkPipelineColorBlendAttachmentState		noBlendAttachment = CreateColorBlendAttachmentState(
				VK_FALSE,
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				VK_BLEND_FACTOR_SRC_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VK_BLEND_OP_ADD,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_FACTOR_ZERO,
				VK_BLEND_OP_ADD
			);
			inline static const VkPipelineColorBlendAttachmentState		defaultBlendAttachment = CreateColorBlendAttachmentState(
				VK_TRUE,
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				VK_BLEND_FACTOR_SRC_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VK_BLEND_OP_ADD,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_FACTOR_ZERO,
				VK_BLEND_OP_ADD
			);
			inline static const VkPipelineColorBlendAttachmentState		additiveBlendAttachment = CreateColorBlendAttachmentState(
				VK_TRUE,
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_OP_ADD,
				VK_BLEND_FACTOR_ONE,
				VK_BLEND_FACTOR_ZERO,
				VK_BLEND_OP_ADD
			);

			// depth stencil states
			inline static const VkPipelineDepthStencilStateCreateInfo	depthCompareWrite = CreateDepthState(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS);
			inline static const VkPipelineDepthStencilStateCreateInfo	depthCompareNoWrite = CreateDepthState(VK_TRUE, VK_FALSE, VK_COMPARE_OP_LESS);

			// color blend states
			inline static const VkPipelineColorBlendStateCreateInfo		noColorBlendState = CreateColorBlendState(0, {});
			inline static const VkPipelineColorBlendStateCreateInfo		defaultColorBlendState = CreateColorBlendState(1, {defaultBlendAttachment});
			inline static const VkPipelineColorBlendStateCreateInfo		additiveColorBlendState = CreateColorBlendState(1, {additiveBlendAttachment});

			// input assembly states
			inline static const VkPipelineInputAssemblyStateCreateInfo	triangleListState = CreateInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
			inline static const VkPipelineInputAssemblyStateCreateInfo	lineListState = CreateInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_LINE_LIST, false);
	};
}