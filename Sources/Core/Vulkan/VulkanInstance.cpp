#include "VulkanInstance.hpp"

#include <set>
#include <array>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <algorithm>

#include GLFW_INCLUDE
#include "Vk.hpp"
#include "VkExt.hpp"

using namespace LWGC;

VulkanInstance *	VulkanInstance::_instanceSingleton;

VulkanInstance *	VulkanInstance::Get(void)
{
	return _instanceSingleton;
}

VulkanInstance::VulkanInstance(void) : VulkanInstance("(null)") {}

VulkanInstance::VulkanInstance(const std::string & applicationName) : _applicationName(applicationName), _enableValidationLayers(false)
{
	_instance = VK_NULL_HANDLE;
	_physicalDevice = VK_NULL_HANDLE;
	_descriptorPool = VK_NULL_HANDLE;
	_device = VK_NULL_HANDLE;
	_applicationName = applicationName;
}

VulkanInstance::VulkanInstance(const std::string & applicationName, const std::vector< const char * > validationLayers, const std::vector< const char * > deviceExtensions) : VulkanInstance(applicationName)
{
	SetDeviceExtensions(deviceExtensions);
	SetValidationLayers(validationLayers);
}

VulkanInstance::~VulkanInstance(void)
{
	std::cout << "desctroyed instance !\n";

	if (_enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(_debugUtilsMessengerCallback, nullptr);
		VkExt::DestroyDebugReportCallbackFunction(_instance, _debugReportCallback, nullptr);
	}

	vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);

	if (_device != VK_NULL_HANDLE)
		vkDestroyDevice(_device, nullptr);
	if (_instance != VK_NULL_HANDLE)
		vkDestroyInstance(_instance, nullptr);
}

void			VulkanInstance::Initialize(void)
{
	_instanceSingleton = this;
	CreateInstance();
	InitializeVulkanFunctions();
	SetupDebugCallbacks();
}


void			VulkanInstance::InitializeVulkanFunctions(void) noexcept
{
	// Debug report (validation layers) extension
	if (std::find(_deviceExtensions.begin(), _deviceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != _deviceExtensions.end())
	{
		VkExt::CreateDebugUtilsMessengerFunction = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(glfwGetInstanceProcAddress(_instance, "vkCreateDebugUtilsMessengerEXT"));
		VkExt::DestroyDebugUtilMessengerFunction = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(glfwGetInstanceProcAddress(_instance, "vkDestroyDebugUtilsMessengerEXT"));
		VkExt::CreateDebugReportCallbackFunction = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(glfwGetInstanceProcAddress(_instance, "vkCreateDebugReportCallbackEXT"));
		VkExt::DestroyDebugReportCallbackFunction = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(glfwGetInstanceProcAddress(_instance, "vkDestroyDebugReportCallbackEXT"));
		VkExt::DebugReportMessageFunction = reinterpret_cast<PFN_vkDebugReportMessageEXT>(glfwGetInstanceProcAddress(_instance, "vkDebugReportMessageEXT"));
	}

	if (std::find(_deviceExtensions.begin(), _deviceExtensions.end(), VK_EXT_DEBUG_MARKER_EXTENSION_NAME) != _deviceExtensions.end())
	{
		// Debug maker extension:
		VkExt::DebugMarkerSetObjectTagFunction = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(glfwGetInstanceProcAddress(_instance, "vkDebugMarkerSetObjectTagEXT"));
		VkExt::DebugMarkerSetObjectNameFunction = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(glfwGetInstanceProcAddress(_instance, "vkDebugMarkerSetObjectNameEXT"));
		VkExt::CmdDebugMarkerBeginFunction = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(glfwGetInstanceProcAddress(_instance, "vkCmdDebugMarkerBeginEXT"));
		VkExt::CmdDebugMarkerEndFunction = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(glfwGetInstanceProcAddress(_instance, "vkCmdDebugMarkerEndEXT"));
		VkExt::CmdDebugMarkerInsertFunction = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(glfwGetInstanceProcAddress(_instance, "vkCmdDebugMarkerInsertEXT"));
	}
}

void			VulkanInstance::InitializeSurface(VkSurfaceKHR surface)
{
	_surface = surface;

	ChoosePhysicalDevice();
	CreateLogicalDevice();
	CreateCommandBufferPools();
	CreateDescriptorPool();

	VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(_physicalDevice, &props);
	printf("Max bindings: %i\n", props.limits.maxVertexInputBindings);
}

void			VulkanInstance::CreateDescriptorPool(void) noexcept
{
	if (_descriptorPool != VK_NULL_HANDLE)
		return ;

	// TODO: refactor this
	std::array<VkDescriptorPoolSize, 5> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 1000u;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[1].descriptorCount = 2u;
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSizes[2].descriptorCount = 6u;
	poolSizes[3].type = VK_DESCRIPTOR_TYPE_SAMPLER;
	poolSizes[3].descriptorCount = 4u;
	poolSizes[4].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	poolSizes[4].descriptorCount = 2u;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 1012u;

	if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	    throw std::runtime_error("failed to create descriptor pool!");
}

void			VulkanInstance::UpdateSurface(void)
{
	InitSurfaceForPhysicalDevice(_physicalDevice);
}

void			VulkanInstance::CreateInstance(void)
{
	if (_validationLayers.size() > 0 && !AreValidationLayerSupported())
		throw std::runtime_error("Validation layers requested but not available !\n");

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = _applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "LWGC";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance !");
}

bool			VulkanInstance::AreValidationLayerSupported(void) noexcept
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : _validationLayers)
	{
	    bool layerFound = false;

	    for (const auto& layerProperties : availableLayers)
	    {
	        if (strcmp(layerName, layerProperties.layerName) == 0)
	        {
	        	layerFound = true;
	        	break;
	        }
	    }

	    if (!layerFound)
		{
			std::cerr << "Validation layer " << layerName << " not found !\n";
	        return false;
		}
	}

	return true;
}

std::vector<const char *>	VulkanInstance::GetRequiredExtensions(void) noexcept
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (_enableValidationLayers)
	    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

DeviceCapability			VulkanInstance::GetDeviceCapability(VkPhysicalDevice physicalDevice) noexcept
{
	DeviceCapability	capability;
	capability.physicalDevice = physicalDevice;

	VkPhysicalDeviceProperties	properties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	capability.deviceName = properties.deviceName;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int index = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, _surface, &presentSupport);

		capability.queues.push_back(DeviceQueue{
			static_cast<uint32_t>(index),
			VK_NULL_HANDLE,
			presentSupport == true,
			(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0
		});
		index++;

		auto g = queueFamily.minImageTransferGranularity;
		std::cout << "queue count:" << queueFamily.queueCount << ", flags: "
			<< queueFamily.queueFlags << ", memory granularity: "
			<< g.width << "/" << g.height << "/" << g.depth << std::endl;
	}

	return capability;
}

void			VulkanInstance::InitSurfaceForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &_surfaceCapabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		_surfaceFormats.resize(formatCount);
	    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, _surfaceFormats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		_surfacePresentModes.resize(presentModeCount);
	    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, _surfacePresentModes.data());
	}
}

DeviceCapability			VulkanInstance::IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice) noexcept
{
	DeviceCapability capability = GetDeviceCapability(physicalDevice);

	capability.supportExtensions = AreExtensionsSupportedForPhysicalDevice(physicalDevice);

	InitSurfaceForPhysicalDevice(physicalDevice);
	capability.supportSurface = !_surfaceFormats.empty() && !_surfacePresentModes.empty();

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

	// Check for decent GPU
	capability.supportedFeatures = supportedFeatures.samplerAnisotropy
		&& supportedFeatures.shaderUniformBufferArrayDynamicIndexing	// Dynamic indexing for bindless
		&& supportedFeatures.depthClamp									// Depth clamp for directional lights
		&& supportedFeatures.imageCubeArray
		&& supportedFeatures.fragmentStoresAndAtomics					// For FPTL ?
		&& supportedFeatures.fillModeNonSolid							// For gizmos
	;

	std::cout << "supportedFeatures.wideLines: " << supportedFeatures.wideLines << std::endl;

	return capability;
}

void			VulkanInstance::ChoosePhysicalDevice(void)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	std::cout << "Found devices count: " << deviceCount << std::endl;

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	if (deviceCount == 0)
		throw std::runtime_error("No GPU detected");

	std::vector< DeviceCapability >	deviceCapabilities;

	for (const auto & device : devices)
	{
		deviceCapabilities.push_back(IsPhysicalDeviceSuitable(device));
	}

	if (deviceCapabilities.size() == 0)
		throw std::runtime_error("failed to find a GPU!");

	// Find the "best" GPU on the machine

	std::sort(deviceCapabilities.begin(), deviceCapabilities.end(), [](const DeviceCapability & c1, const DeviceCapability & c2){
		return c1.GetGPUScore() < c2.GetGPUScore();
	});

	auto & bestDevice = deviceCapabilities[0];

	if (bestDevice.GetGPUScore() == -1)
		throw std::runtime_error("Can't find a GPU that met requirement");

	// setup choosen device
	_physicalDevice = bestDevice.physicalDevice;
	_availableQueues = bestDevice.queues;

	std::cout << "Choosed " << bestDevice.deviceName << " as the best GPU on your machine." << std::endl;
}

void			VulkanInstance::CreateLogicalDevice(void)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {_availableQueues[0].index};

	float queuePriority = 1.0f;
	for (DeviceQueue deviceQueue : _availableQueues)
	{
	    VkDeviceQueueCreateInfo queueCreateInfo = {};
	    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	    queueCreateInfo.queueFamilyIndex = deviceQueue.index;
	    queueCreateInfo.queueCount = 1;
	    queueCreateInfo.pQueuePriorities = &queuePriority;
	    queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.fillModeNonSolid = VK_TRUE;
	deviceFeatures.depthClamp = VK_TRUE;
	deviceFeatures.depthBiasClamp = VK_TRUE;
	deviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
	deviceFeatures.imageCubeArray = VK_TRUE;
	deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
	deviceFeatures.multiViewport = VK_TRUE;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = _deviceExtensions.data();

	std::cout << "Enabled vulkan extensions: ";
	for (auto extension : _deviceExtensions)
		std::cout << extension << ((extension != _deviceExtensions.back()) ? ", " : "");
	std::cout << std::endl;

	if (_enableValidationLayers) {
	    createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
	    createInfo.ppEnabledLayerNames = _validationLayers.data();
	} else {
	    createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create logical device!");
	}

	AllocateDeviceQueue(_mainQueue.queue, _mainQueue.index);

	printf("Create logical device !\n");
}

void			VulkanInstance::AllocateDeviceQueue(VkQueue & queue, uint32_t & queueIndex)
{
	auto unallocatedDeviceQueue = std::find_if(_availableQueues.begin(), _availableQueues.end(), [](const auto & deviceQueue){
		return deviceQueue.queue == VK_NULL_HANDLE;
	});

	// If all the device queue have been allocated, we just return the main queue
	if (unallocatedDeviceQueue == _availableQueues.end())
	{
		std::cout << "Warning: too many allocated queues, returning the main queue." << std::endl;
		queue = _mainQueue.queue;
		queueIndex = _mainQueue.index;
		return ;
	}

	vkGetDeviceQueue(_device, unallocatedDeviceQueue->index, 0, &unallocatedDeviceQueue->queue);

	queue = unallocatedDeviceQueue->queue;
	queueIndex = unallocatedDeviceQueue->index;
}

uint32_t		VulkanInstance::GetAvailableDevceQueueCount(void)
{
	return std::count_if(_availableQueues.begin(), _availableQueues.end(), [](const DeviceQueue & deviceQueue) {
		return deviceQueue.queue != VK_NULL_HANDLE;
	});
}

void			VulkanInstance::CreateCommandBufferPools(void) noexcept
{
	_commandBufferPool.Initialize(_mainQueue.queue, _mainQueue.index);
}

bool			VulkanInstance::AreExtensionsSupportedForPhysicalDevice(VkPhysicalDevice physicalDevice) noexcept
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set< std::string > requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

	for (const auto & extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	if (requiredExtensions.size() != 0)
	{
		std::cout << "Missing required extensions: ";
		for (const auto & extension : requiredExtensions)
			std::cout << extension << ", ";
		std::cout << std::endl;
	}

	return requiredExtensions.empty();
}

uint32_t		VulkanInstance::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		    return i;
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

VkFormat		VulkanInstance::FindSupportedFormat(const std::vector< VkFormat > & candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
	    VkFormatProperties properties;
	    vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &properties);
	    if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
	        return format;
	    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
	        return format;
	    }
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat		VulkanInstance::FindDepthFormat(void)
{
	return FindSupportedFormat(
			{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
}

void		VulkanInstance::SetValidationLayers(const std::vector< const char * > validationLayers) noexcept
{
	_validationLayers = validationLayers;
	_enableValidationLayers = true;
}

void		VulkanInstance::SetDeviceExtensions(const std::vector< const char * > deviceExtensions) noexcept
{
	_deviceExtensions = deviceExtensions;
}

void		VulkanInstance::SetApplicationName(const std::string & applicationName) noexcept
{
	_applicationName = applicationName;
}

VkInstance	VulkanInstance::GetInstance(void) const noexcept { return (this->_instance); }

VkQueue		VulkanInstance::GetQueue(void) const noexcept { return (this->_mainQueue.queue); }

uint32_t	VulkanInstance::GetQueueIndex(void) const noexcept { return (this->_mainQueue.index); }

const std::vector< VkSurfaceFormatKHR >	VulkanInstance::GetSupportedSurfaceFormats(void) const noexcept { return (this->_surfaceFormats); }
const std::vector< VkPresentModeKHR >	VulkanInstance::GetSupportedPresentModes(void) const noexcept { return (this->_surfacePresentModes); }
const VkSurfaceCapabilitiesKHR			VulkanInstance::GetSurfaceCapabilities(void) const noexcept { return (this->_surfaceCapabilities); }

VkPhysicalDevice	VulkanInstance::GetPhysicalDevice(void) const noexcept { return (this->_physicalDevice); }
VkDevice			VulkanInstance::GetDevice(void) const noexcept { return (this->_device); }

CommandBufferPool *	VulkanInstance::GetCommandBufferPool(void) noexcept { return &this->_commandBufferPool; }

VkDescriptorPool	VulkanInstance::GetDescriptorPool(void) const noexcept
{
	return _descriptorPool;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	(void)messageSeverity;
	(void)messageType;
	(void)pUserData;
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

// Message callback debug from Sascha Willems: https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanDebug.cpp
VKAPI_ATTR VkBool32 VKAPI_CALL messageCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
{
	(void)pUserData, (void)location, (void)srcObject, (void)objType;

	// Select prefix depending on flags passed to the callback
	// Note that multiple flags may be set for a single validation message
	std::string prefix("");

	// Error that may result in undefined behaviour
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		prefix += "ERROR:";
	};
	// Warnings may hint at unexpected / non-spec API usage
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		prefix += "WARNING:";
	};
	// May indicate sub-optimal usage of the API
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		prefix += "PERFORMANCE:";
	};
	// Informal messages that may become handy during debugging
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		prefix += "INFO:";
	}
	// Diagnostic info from the Vulkan loader and layers
	// Usually not helpful in terms of API usage, but may help to debug layer and loader problems
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		prefix += "DEBUG:";
	}

	// Display message to default output (console/logcat)
	std::stringstream debugMessage;
	debugMessage << prefix << " [" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

#if defined(__ANDROID__)
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		LOGE("%s", debugMessage.str().c_str());
	}
	else {
		LOGD("%s", debugMessage.str().c_str());
	}
#else
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cerr << debugMessage.str() << "\n";
	}
	else {
		std::cout << debugMessage.str() << "\n";
	}
#endif

	fflush(stdout);

	// The return value of this callback controls wether the Vulkan call that caused
	// the validation message will be aborted or not
	// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message
	// (and return a VkResult) to abort
	// If you instead want to have calls abort, pass in VK_TRUE and the function will
	// return VK_ERROR_VALIDATION_FAILED_EXT
	return VK_FALSE;
}

void		VulkanInstance::SetupDebugCallbacks(void) noexcept
{
	// TODO: setup the DebugReportMessageFunction as well (VK_EXT_debug_report)
	if (VkExt::AreDebugLayerAvailable())
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		Vk::CheckResult(CreateDebugUtilsMessengerEXT(&createInfo, nullptr, &_debugUtilsMessengerCallback), "Failed to set up debug callback!");

		VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
		dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)messageCallback;
		dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

		Vk::CheckResult(VkExt::CreateDebugReportCallbackFunction(
			_instance,
			&dbgCreateInfo,
			nullptr,
			&_debugReportCallback), "Failed to register the debug report callback");
	}
}

VkResult	VulkanInstance::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) noexcept
{
	if (VkExt::CreateDebugUtilsMessengerFunction != nullptr)
	    return VkExt::CreateDebugUtilsMessengerFunction(_instance, pCreateInfo, pAllocator, pCallback);
	else
	    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void		VulkanInstance::DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
{
	if (VkExt::DestroyDebugUtilMessengerFunction != nullptr)
		VkExt::DestroyDebugUtilMessengerFunction(_instance, callback, pAllocator);
}

int			DeviceCapability::GetGPUScore(void) const
{
	if (!supportedFeatures || !supportExtensions || !supportSurface || queues.size() <= 1)
	{
		std::cout << "supported features: " << supportedFeatures << ", " << supportExtensions << ", " << supportSurface << ", " << queues.size() << std::endl;
		return -1;
	}

	int score = queues.size();

	for (const auto queue : queues)
	{
		score += queue.supportCompute;
	}

	return score;
}

std::ostream &	operator<<(std::ostream & o, VulkanInstance const & r)
{
	o << "Vulkan Instance" << std::endl;
	(void)r;
	return (o);
}
