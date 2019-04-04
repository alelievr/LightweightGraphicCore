#include "IncludeDeps.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

#include VULKAN_INCLUDE
#include GLFW_INCLUDE

#define STR(x) #x
#define CAT(x1, x2) x1##x2
#define LOAD(functionName) functionName = reinterpret_cast<CAT(PFN_,functionName)>(glfwGetInstanceProcAddress(instance, STR(functionName)))
#define DECL(functionName) static inline CAT(PFN_,functionName) functionName

namespace LWGC
{
	// Report callbacks (validation layers)
	DECL(vkDebugReportMessageEXT);
	DECL(vkCreateDebugReportCallbackEXT);
	DECL(vkDestroyDebugReportCallbackEXT);
	DECL(vkCreateDebugUtilsMessengerEXT);
	DECL(vkDestroyDebugUtilsMessengerEXT);

	// Debug markers:
	DECL(vkDebugMarkerSetObjectTagEXT);
	DECL(vkDebugMarkerSetObjectNameEXT);
	DECL(vkCmdDebugMarkerBeginEXT);
	DECL(vkCmdDebugMarkerEndEXT);
	DECL(vkCmdDebugMarkerInsertEXT);

	// Ray tracing:
	DECL(vkCreateAccelerationStructureNV);

	// Cooperative matrices:
	DECL(vkGetPhysicalDeviceCooperativeMatrixPropertiesNV);
	DECL(vkDestroyAccelerationStructureNV);
	DECL(vkGetAccelerationStructureMemoryRequirementsNV);
	DECL(vkBindAccelerationStructureMemoryNV);
	DECL(vkCmdBuildAccelerationStructureNV);
	DECL(vkCmdCopyAccelerationStructureNV);
	DECL(vkCmdTraceRaysNV);
	DECL(vkCreateRayTracingPipelinesNV);
	DECL(vkGetRayTracingShaderGroupHandlesNV);
	DECL(vkGetAccelerationStructureHandleNV);
	DECL(vkCmdWriteAccelerationStructuresPropertiesNV);
	DECL(vkCompileDeferredNV);

	// Store the vulkan extension function pointers
	struct VkExt
	{
		static inline bool AreDebugMarkersAvailable()
		{
			return vkDebugMarkerSetObjectTagEXT != VK_NULL_HANDLE;
		}

		static inline bool AreDebugLayerAvailable()
		{
			return vkDebugReportMessageEXT != VK_NULL_HANDLE;
		}

		static inline bool IsRayTracingAvailable()
		{
			return vkCreateAccelerationStructureNV != VK_NULL_HANDLE;
		}

		static inline bool AreCooperativeMatricesAvailable()
		{
			return vkGetPhysicalDeviceCooperativeMatrixPropertiesNV != VK_NULL_HANDLE;
		}

		friend class VulkanInstance;
		private:
			static inline void LoadDebugMarkers(void)
			{
				VkInstance instance = VulkanInstance::Get()->GetInstance();

				// Debug maker extension:
				LOAD(vkDebugMarkerSetObjectTagEXT);
				LOAD(vkDebugMarkerSetObjectNameEXT);
				LOAD(vkCmdDebugMarkerBeginEXT);
				LOAD(vkCmdDebugMarkerEndEXT);
				LOAD(vkCmdDebugMarkerInsertEXT);
			}

			static inline void LoadDebugLayers(void)
			{
				VkInstance instance = VulkanInstance::Get()->GetInstance();

				LOAD(vkCreateDebugUtilsMessengerEXT);
				LOAD(vkDestroyDebugUtilsMessengerEXT);
				LOAD(vkCreateDebugReportCallbackEXT);
				LOAD(vkDestroyDebugReportCallbackEXT);
				LOAD(vkDebugReportMessageEXT);
			}

			static inline void LoadRayTracing(void)
			{
				VkInstance instance = VulkanInstance::Get()->GetInstance();

				LOAD(vkCreateAccelerationStructureNV);
			}

			static inline void LoadCooperativeMatrices(void)
			{
				VkInstance instance = VulkanInstance::Get()->GetInstance();

				LOAD(vkGetPhysicalDeviceCooperativeMatrixPropertiesNV);
				LOAD(vkDestroyAccelerationStructureNV);
				LOAD(vkGetAccelerationStructureMemoryRequirementsNV);
				LOAD(vkBindAccelerationStructureMemoryNV);
				LOAD(vkCmdBuildAccelerationStructureNV);
				LOAD(vkCmdCopyAccelerationStructureNV);
				LOAD(vkCmdTraceRaysNV);
				LOAD(vkCreateRayTracingPipelinesNV);
				LOAD(vkGetRayTracingShaderGroupHandlesNV);
				LOAD(vkGetAccelerationStructureHandleNV);
				LOAD(vkCmdWriteAccelerationStructuresPropertiesNV);
				LOAD(vkCompileDeferredNV);
			}
	};
}

#undef LOAD
#undef CAT
#undef STR