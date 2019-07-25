#pragma once
#include "PXPCH.h"
#include "Window.h"
Window::Window(int width, int height, char* title)
{
	if (!glfwInit())
	{
		Log::GetCoreLogger()->error("failed window done!");
		return;
	}
	origin = glfwCreateWindow(width, height, title, nullptr, nullptr);
	CreateVkInstance();
	PickPhysicalDevice();
}


void Window::OnUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(origin);
	glfwPollEvents();
}

void Window::OnInit()
{
	glfwSetWindowUserPointer(origin, &windowData);
	glfwSetKeyCallback(origin, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto m_data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS:
		{
			Log::GetClientLogger()->info(key);
			KeyPresssedEvent event(key);
			m_data.EventCallback(event);
			break;
		}
		default:
			break;
		}
	});
}

void Window::OnDestroy()
{
}

Window::~Window()
{
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(origin);
	glfwTerminate();
}

void Window::CreateVkInstance()
{
	CheckVkExtensions();
	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pApplicationName = "Hello world - losuffi!";
	appinfo.pEngineName = "Paxel";
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createinfo = {};
	createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createinfo.pApplicationInfo = &appinfo;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtension;
	glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	Log::GetCoreLogger()->info("glfw require extensions:");
	for (int i =0; i < glfwExtensionCount; i++)
	{
		Log::GetCoreLogger()->info(*(glfwExtension + i));
	}
	createinfo.enabledExtensionCount = glfwExtensionCount;
	createinfo.ppEnabledExtensionNames = glfwExtension;
	createinfo.enabledLayerCount = 0;
	VkResult result = vkCreateInstance(&createinfo, nullptr, &instance);
}

void Window::CheckVkExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	Log::GetCoreLogger()->info("avaliable extensions:");
	for (const auto& extension : extensions)
	{
		Log::GetCoreLogger()->info(extension.extensionName);
	}
}

void Window::PickPhysicalDevice()
{
	physicalDevice = VK_NULL_HANDLE;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		Log::GetCoreLogger()->error("Failed to find GPUs With Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	for (const auto& dev : devices)
	{
		if (isSuitableDevice(dev))
		{
			physicalDevice = dev;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE)
	{
		Log::GetCoreLogger()->error("failed to find a suitable GPU!");
	}
}

void Window::CreateLogicDevice()
{
	
}

bool Window::isSuitableDevice(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	Log::GetCoreLogger()->debug(deviceProperties.deviceName);
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}
