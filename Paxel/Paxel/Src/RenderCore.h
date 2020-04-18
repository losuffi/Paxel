#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include "PXPCH.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "Core.h"

struct QueueFamilyIndics 
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct VkRenderCoreInfoList
{
	const VkInstance instance;
	const VkPhysicalDevice physicalDevice;
	const VkDevice device;
	const VkQueue graphicsQueue;
	const VkQueue presentQueue;
	const VkSurfaceKHR surface;
	const QueueFamilyIndics familyIndics;
	VkRenderCoreInfoList(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device, VkQueue graphics_queue,
		VkQueue present_queue, VkSurfaceKHR surface, QueueFamilyIndics Indics)
		: instance(instance),
		  physicalDevice(physical_device),
		  device(device),
		  graphicsQueue(graphics_queue),
		  presentQueue(present_queue),
		  surface(surface),
		  familyIndics(Indics)
	{
	}
};

class RenderCore
{
public:
	void OnInit(GLFWwindow* wind);
	void OnDestroy();
	[[nodiscard]] VkRenderCoreInfoList GetInfoList() const;
protected:
	void CreateVkInstance();
	void CheckVkExtensions();
	void PickPhysicalDevice();
	void CreateLogicDevice();
	bool isSuitableDevice(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews(VkFormat ImageViewFormat);
	
	QueueFamilyIndics FindQueueFamilies(VkPhysicalDevice device) const;
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
protected:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice PhysicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageviews;
};

