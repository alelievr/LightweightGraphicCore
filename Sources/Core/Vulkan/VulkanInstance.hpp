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

	class		VulkanInstance
	{
		friend class VulkanSurface;

		private:
			VkInstance					_instance;
			VkPhysicalDevice			_physicalDevice;
			VkDevice					_device;
			std::string					_applicationName;
			bool						_enableValidationLayers;
			VkDebugUtilsMessengerEXT	_callback;
			VkDescriptorPool			_descriptorPool;

			// uint32_t					_graphicQueueIndex;
			// VkQueue						_graphicQueue;
			// uint32_t					_presentQueueIndex;
			// VkQueue						_presentQueue;
			// uint32_t					_computeQueueIndex;
			// VkQueue						_computeQueue;
			uint32_t					_queueIndex;
			VkQueue						_queue;

			VkSurfaceKHR				_surface;
			VkSurfaceCapabilitiesKHR	_surfaceCapabilities;
			std::vector< VkSurfaceFormatKHR >	_surfaceFormats;
			std::vector< VkPresentModeKHR >		_surfacePresentModes;

			std::vector< const char * >	_validationLayers;
			std::vector< const char * >	_deviceExtensions;

			CommandBufferPool			_commandBufferPool;
			// CommandBufferPool			_graphicCommandBufferPool;
			// CommandBufferPool			_computeCommandBufferPool;

			static VulkanInstance *		_instanceSingleton;

			void		SetupDebugCallbacks(void) noexcept;
			bool		AreValidationLayerSupported(void) noexcept;
			void		ChoosePhysicalDevice(void);
			bool		IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice) noexcept;
			void		InitQueueIndicesForPhysicalDevice(VkPhysicalDevice device) noexcept;
			void		InitSurfaceForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept;
			bool		AreExtensionsSupportedForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept;
			void		CreateInstance(void);
			void		CreateLogicalDevice(void);
			void		CreateCommandBufferPools(void) noexcept;
			void		CreateDescriptorPool(void) noexcept;
			std::vector<const char *>	GetRequiredExtensions(void) noexcept;

			VkResult	CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) noexcept;
			void		DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
			void		InitializeSurface(VkSurfaceKHR surface);

		public:
			VulkanInstance(void);
			VulkanInstance(const std::string & applicationName);
			VulkanInstance(const std::string & applicationName, const std::vector< const char * > validationLayers, const std::vector< const char * > deviceExtensions);
			VulkanInstance(const VulkanInstance &) = delete;
			virtual ~VulkanInstance(void);

			VulkanInstance &	operator=(VulkanInstance const & src) = delete;

			void		SetValidationLayers(const std::vector< const char * > validationLayers) noexcept;
			void		SetDeviceExtensions(const std::vector< const char * > deviceExtensions) noexcept;
			void		SetApplicationName(const std::string & applicationName) noexcept;

			void		Initialize(void);
			void		UpdateSurface(void);

			VkInstance	GetInstance(void) const noexcept;

			VkQueue		GetQueue(void) const noexcept;
			// VkQueue		GetGraphicQueue(void) const noexcept;
			// VkQueue		GetPresentQueue(void) const noexcept;
			// VkQueue		GetComputeQueue(void) const noexcept;

			uint32_t	GetQueueIndex(void) const noexcept;
			// uint32_t	GetGraphicQueueIndex(void) const noexcept;
			// uint32_t	GetPresentQueueIndex(void) const noexcept;
			// uint32_t	GetComputeQueueIndex(void) const noexcept;

			CommandBufferPool *	GetCommandBufferPool(void) noexcept;
			// CommandBufferPool *	GetGraphicCommandBufferPool(void) noexcept;
			// CommandBufferPool *	GetComputeCommandBufferPool(void) noexcept;

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
			VkQueue		AllocateDeviceQueue(void);

			// Instance singleton
			static VulkanInstance *		Get(void);
	};

	std::ostream &	operator<<(std::ostream & o, VulkanInstance const & r);
}