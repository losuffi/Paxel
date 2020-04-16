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

class RenderCore
{
public:
	void OnInit(GLFWwindow* wind);
	void OnDestroy();
protected:
	void CreateVkInstance();
	void CheckVkExtensions();
	void PickPhysicalDevice();
	void CreateLogicDevice();
	bool isSuitableDevice(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateSurface();
	QueueFamilyIndics FindQueueFamilies(VkPhysicalDevice device);
protected:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
};

