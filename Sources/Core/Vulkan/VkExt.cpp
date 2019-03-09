#include "VkExt.hpp"

using namespace LWGC;

// Garentied to be null because they're static
PFN_vkCreateDebugReportCallbackEXT	VkExt::CreateDebugReportCallbackFunction;
PFN_vkDestroyDebugReportCallbackEXT	VkExt::DestroyDebugReportCallbackFunction;
PFN_vkCreateDebugUtilsMessengerEXT	VkExt::CreateDebugUtilsMessengerFunction;
PFN_vkDestroyDebugUtilsMessengerEXT	VkExt::DestroyDebugUtilMessengerFunction;
PFN_vkDebugReportMessageEXT			VkExt::DebugReportMessageFunction;

// Debug markers:
PFN_vkDebugMarkerSetObjectTagEXT	VkExt::DebugMarkerSetObjectTagFunction;
PFN_vkDebugMarkerSetObjectNameEXT	VkExt::DebugMarkerSetObjectNameFunction;
PFN_vkCmdDebugMarkerBeginEXT		VkExt::CmdDebugMarkerBeginFunction;
PFN_vkCmdDebugMarkerEndEXT			VkExt::CmdDebugMarkerEndFunction;
PFN_vkCmdDebugMarkerInsertEXT		VkExt::CmdDebugMarkerInsertFunction;

bool		VkExt::AreDebugMarkersAvailable()
{
	return DebugMarkerSetObjectTagFunction != VK_NULL_HANDLE
		&& DebugMarkerSetObjectNameFunction != VK_NULL_HANDLE
		&& CmdDebugMarkerBeginFunction != VK_NULL_HANDLE
		&& CmdDebugMarkerEndFunction != VK_NULL_HANDLE
		&& CmdDebugMarkerInsertFunction != VK_NULL_HANDLE;
}

bool		VkExt::AreDebugLayerAvailable()
{
	return CreateDebugReportCallbackFunction != VK_NULL_HANDLE
		&& DestroyDebugReportCallbackFunction != VK_NULL_HANDLE
		&& CreateDebugUtilsMessengerFunction != VK_NULL_HANDLE
		&& DestroyDebugUtilMessengerFunction != VK_NULL_HANDLE
		&& DebugReportMessageFunction != VK_NULL_HANDLE;
}