#include "PXPCH.h"
#include "RenderCore.h"
void RenderCore::OnInit(GLFWwindow* wind)
{
	window = wind;
	CreateVkInstance();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicDevice();
}

void RenderCore::OnDestroy()
{
	vkDestroyInstance(instance, nullptr);
}

void RenderCore::CreateVkInstance()
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

void RenderCore::CheckVkExtensions()
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

void RenderCore::PickPhysicalDevice()
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

void RenderCore::CreateLogicDevice()
{
	QueueFamilyIndics indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			queueFamily,
			1,
			&queuePriority
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		Log::GetCoreLogger()->error("failed to create logical device!");
	}
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

bool RenderCore::isSuitableDevice(VkPhysicalDevice device)
{
	QueueFamilyIndics indices = FindQueueFamilies(device);
	return indices.IsComplete();
}

void RenderCore::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		nullptr,
		0,
		GetModuleHandle(nullptr),
		glfwGetWin32Window(window),
	};
	if (vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &surface)!=VK_SUCCESS)
	{
		Log::GetCoreLogger()->error("Can't Create Win32 Surface!");
	}
}

QueueFamilyIndics RenderCore::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndics indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport)
		{
			indices.presentFamily = i;
		}
		if (indices.IsComplete())
		{
			break;
		}
		i++;
	}
	return indices;
}
