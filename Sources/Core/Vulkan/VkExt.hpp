#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE

namespace LWGC
{
	// Store the vulkan extension function pointers
	struct VkExt
	{
		// Report callbacks (validation layers)
		static PFN_vkDebugReportMessageEXT			DebugReportMessageFunction;
		static PFN_vkCreateDebugReportCallbackEXT	CreateDebugReportCallbackFunction;
		static PFN_vkDestroyDebugReportCallbackEXT	DestroyDebugReportCallbackFunction;
		static PFN_vkCreateDebugUtilsMessengerEXT	CreateDebugUtilsMessengerFunction;
		static PFN_vkDestroyDebugUtilsMessengerEXT	DestroyDebugUtilMessengerFunction;

		// Debug markers:
		static PFN_vkDebugMarkerSetObjectTagEXT		DebugMarkerSetObjectTagFunction;
		static PFN_vkDebugMarkerSetObjectNameEXT	DebugMarkerSetObjectNameFunction;
		static PFN_vkCmdDebugMarkerBeginEXT			CmdDebugMarkerBeginFunction;
		static PFN_vkCmdDebugMarkerEndEXT			CmdDebugMarkerEndFunction;
		static PFN_vkCmdDebugMarkerInsertEXT		CmdDebugMarkerInsertFunction;

		static bool		AreDebugMarkersAvailable();
		static bool		AreDebugLayerAvailable();
	};
}