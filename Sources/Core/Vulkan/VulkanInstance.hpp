#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IncludeDeps.hpp"

#include VULKAN_INCLUDE
#include "CommandBufferPool.hpp"

namespace LWGC
{
	class VulkanSurface;

	struct DeviceQueue
	{
		uint32_t				index;					// aka queue family index
		std::vector< VkQueue >	queues;
		uint32_t				count;					// number of available queue of this family
		uint32_t				allocatedQueueCount;
		bool					supportPresent;
		bool					supportCompute;
	};

	struct DeviceCapability
	{
		bool				supportedFeatures;
		bool				supportSurface;
		VkPhysicalDevice	physicalDevice;
		std::vector< DeviceQueue >	queues;
		std::vector< std::string >	enabledExtensions;
		std::string			deviceName;

		int		GetGPUScore(void) const;
	};

	class		VulkanInstance
	{
		friend class VulkanSurface;

		private:
			VkInstance					_instance;
			VkPhysicalDevice			_physicalDevice;
			VkDevice					_device;
			std::string					_applicationName;
			bool						_enableValidationLayers;
			VkDebugUtilsMessengerEXT	_debugUtilsMessengerCallback;
			VkDebugReportCallbackEXT	_debugReportCallback;
			VkDescriptorPool			_descriptorPool;

			VkQueue						_queue;

			DeviceQueue					_mainQueue;
			std::vector< DeviceQueue >	_availableQueues;

			VkSurfaceKHR				_surface;
			VkSurfaceCapabilitiesKHR	_surfaceCapabilities;
			std::vector< VkSurfaceFormatKHR >	_surfaceFormats;
			std::vector< VkPresentModeKHR >		_surfacePresentModes;

			std::vector< const char * >	_validationLayers;
			std::vector< std::string >	_deviceExtensions;

			CommandBufferPool			_commandBufferPool;

			static VulkanInstance *		_instanceSingleton;

			void		SetupDebugCallbacks(void) noexcept;
			bool		AreValidationLayerSupported(void) noexcept;
			void		ChoosePhysicalDevice(void);
			DeviceCapability	IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice) noexcept;
			void		InitQueueIndicesForPhysicalDevice(VkPhysicalDevice device) noexcept;
			std::vector< std::string >	AreExtensionsSupportedForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept;
			void		CreateInstance(void);
			void		CreateLogicalDevice(void);
			void		CreateCommandBufferPools(void) noexcept;
			void		CreateDescriptorPool(void);
			std::vector<const char *>	GetRequiredExtensions(void) noexcept;
			DeviceCapability	GetDeviceCapability(VkPhysicalDevice physicalDevice) noexcept;

			VkResult	CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) noexcept;
			void		DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
			void		InitializeSurface(VkSurfaceKHR surface);
			void		InitSurfaceForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept;

		public:
			VulkanInstance(void);
			VulkanInstance(const std::string & applicationName);
			VulkanInstance(const std::string & applicationName, const std::vector< const char * > validationLayers, const std::vector< std::string > deviceExtensions);
			VulkanInstance(const VulkanInstance &) = delete;
			virtual ~VulkanInstance(void);

			VulkanInstance &	operator=(VulkanInstance const & src) = delete;

			void		SetValidationLayers(const std::vector< const char * > validationLayers) noexcept;
			void		SetDeviceExtensions(const std::vector< std::string > deviceExtensions) noexcept;
			void		SetApplicationName(const std::string & applicationName) noexcept;

			void		Initialize(void);
			void		UpdateSurface(void);

			VkInstance	GetInstance(void) const noexcept;

			VkQueue		GetQueue(void) const noexcept;
			uint32_t	GetQueueIndex(void) const noexcept;

			CommandBufferPool *	GetCommandBufferPool(void) noexcept;

			const std::vector< VkSurfaceFormatKHR >	GetSupportedSurfaceFormats(void) const noexcept;
			const std::vector< VkPresentModeKHR >	GetSupportedPresentModes(void) const noexcept;
			const VkSurfaceCapabilitiesKHR			GetSurfaceCapabilities(void) const noexcept;

			VkDescriptorPool	GetDescriptorPool(void) const noexcept;

			VkPhysicalDevice	GetPhysicalDevice(void) const noexcept;
			VkDevice			GetDevice(void) const noexcept;

			// Utils
			uint32_t	FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			VkFormat	FindSupportedFormat(const std::vector< VkFormat > & candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			VkFormat	FindDepthFormat(void);
			uint32_t	GetAvailableDevceQueueCount(void);
			void		AllocateDeviceQueue(VkQueue & queue, uint32_t & queueIndex);

			// Instance singleton
			static VulkanInstance *		Get(void);
			
			static bool IsExtensionEnabled(const std::string & extensionName);
			static bool	AreDebugMarkersEnabled(void);
			static bool	IsDebugLayerEnabled(void);
			static bool	IsRayTracingEnabled(void);
			static bool	AreCooperativeMatricesEnabled(void);
	};

	std::ostream &	operator<<(std::ostream & o, VulkanInstance const & r);
}