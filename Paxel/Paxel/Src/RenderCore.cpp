#include "PXPCH.h"
#include "RenderCore.h"
void RenderCore::OnInit(GLFWwindow* wind)
{
	window = wind;
	CreateVkInstance();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicDevice();
	CreateSwapChain();
}

void RenderCore::OnDestroy()
{
	for(auto imageView : swapchainImageviews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance,nullptr);
}

VkRenderCoreInfoList RenderCore::GetInfoList() const
{
	const auto indics = FindQueueFamilies(PhysicalDevice);
	return VkRenderCoreInfoList{
		instance, PhysicalDevice, device,
		graphicsQueue, presentQueue, surface,
		indics, swapchainImageviews.size()};
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
	PX_CORE_INFO("avaliable extensions:");
	for (const auto& extension : extensions)
	{
		PX_CORE_INFO(extension.extensionName);
	}
}

void RenderCore::PickPhysicalDevice()
{
	PhysicalDevice = VK_NULL_HANDLE;
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
			PhysicalDevice = dev;
			break;
		}
	}
	if (PhysicalDevice == VK_NULL_HANDLE)
	{
		Log::GetCoreLogger()->error("failed to find a suitable GPU!");
	}
}

void RenderCore::CreateLogicDevice()
{
	QueueFamilyIndics indices = FindQueueFamilies(PhysicalDevice);

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

	if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
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

bool RenderCore::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);

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
	PX_ENSURE_RET_VOID(vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &surface) == VK_SUCCESS,
		"Can't Create Win32 Surface!");
}

void RenderCore::CreateSwapChain()
{
	const auto SwapChainSupport = QuerySwapChainSupport(PhysicalDevice);
	PX_ENSURE_RET_VOID(!SwapChainSupport.formats.empty(), "SwapChain Format Get Is Empty!");
	PX_ENSURE_RET_VOID(!SwapChainSupport.presentModes.empty(), "SwapChain PresentModes Get Is Empty!");
	auto ChoseSwapExtent = [&](const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if(capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { 1080,680 };
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	};
	swapchainExtent = ChoseSwapExtent(SwapChainSupport.capabilities);
	uint32_t ImageCount = SwapChainSupport.capabilities.minImageCount + 1;

	if(SwapChainSupport.capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.capabilities.maxImageCount)
	{
		ImageCount = SwapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.imageFormat = SwapChainSupport.formats[0].format;
	CreateInfo.imageColorSpace = SwapChainSupport.formats[0].colorSpace;
	CreateInfo.imageExtent = swapchainExtent;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	CreateInfo.preTransform = SwapChainSupport.capabilities.currentTransform;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.presentMode = SwapChainSupport.presentModes[0];
	CreateInfo.clipped = VK_TRUE;

	CreateInfo.oldSwapchain = VK_NULL_HANDLE;

	PX_ENSURE_RET_VOID(vkCreateSwapchainKHR(device, &CreateInfo, nullptr, &swapchain) == VK_SUCCESS,
		"failed to create swap chain!");
	vkGetSwapchainImagesKHR(device, swapchain, &ImageCount, nullptr);
	swapchainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &ImageCount, swapchainImages.data());
	CreateImageViews(SwapChainSupport.formats[0].format);
}

void RenderCore::CreateImageViews(VkFormat ImageViewFormat)
{
	swapchainImageviews.resize(swapchainImages.size());

	for(size_t i = 0; i < swapchainImages.size(); ++i)
	{
		VkImageViewCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.image = swapchainImages[i];
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.format = ImageViewFormat;
		CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		CreateInfo.subresourceRange.baseMipLevel = 0;
		CreateInfo.subresourceRange.levelCount = 1;
		CreateInfo.subresourceRange.baseArrayLayer = 0;
		CreateInfo.subresourceRange.layerCount = 1;
		PX_ENSURE_RET_VOID(vkCreateImageView(device, &CreateInfo, nullptr, &swapchainImageviews[i]) == VK_SUCCESS,
			"Failed to create image views!");
	}
}

VkShaderModule RenderCore::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	CreateInfo.codeSize = code.size();
	CreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule ShaderModule;
	PX_ENSURE_RET_VAL(vkCreateShaderModule(device, &CreateInfo, nullptr, &ShaderModule) == VK_SUCCESS, nullptr, "Failed to create shader module!");
	return ShaderModule;
}

void RenderCore::CreateGraphicPipeline(const std::vector<char>& VertShaderCode,const std::vector<char>& FragShaderCode)
{
	//Shader Specify
	const VkShaderModule VertShaderModule = CreateShaderModule(VertShaderCode);
	const VkShaderModule FragShaderModule = CreateShaderModule(FragShaderCode);
	PX_RENDER_GENERATE_SHADER_STAGE_INFO(VertShaderStageInfo, VK_SHADER_STAGE_VERTEX_BIT, VertShaderModule, main);
	PX_RENDER_GENERATE_SHADER_STAGE_INFO(FragShaderStageInfo, VK_SHADER_STAGE_FRAGMENT_BIT, FragShaderModule, main);

	VkPipelineShaderStageCreateInfo ShaderStages[] = { VertShaderStageInfo, FragShaderStageInfo };
	//Vertex Input & Topology
	PX_RENDER_GENERATE_VERTEX_INPUT_STATE_INFO(VertexInputInfo, 0, 0);
	PX_RENDER_GENERATE_INPUT_ASSEMBLY_STATE_INFO(InputAssembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,VK_FALSE);

	//Viewport and Scissor
	VkViewport Viewport{ 0,0,static_cast<float>(swapchainExtent.width),
		static_cast<float>(swapchainExtent.height),0,1.0f };
	VkRect2D Scissor{};
	Scissor.extent = swapchainExtent;
	Scissor.offset = { 0, 0 };
	PX_RENDER_GENERATE_VIEWPORT_STATE_INFO(ViewportState, 1, &Viewport, 1, &Scissor);

	/*Rasterization setting
		1.rasterzerDiscardEnable:
			if rasterzerDiscardEnable == VK_TRUE, then geometry never passes the rasterization stage.
			Basically disables any output to framebuffer;
		2.frontFace:
			specifies the vertex order for face to be considered front-facing and can be clockwise or counterclockwise.
	*/
	PX_RENDER_GENERATE_RASTERIZATION_STATE_INFO(Rasterization, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL,
		1.0f, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, VK_FALSE);

	// Multi Sample
	PX_RENDER_GENERATE_MULTI_SAMPLE_STATE_INFO(MultiSampling, VK_FALSE, VK_SAMPLE_COUNT_1_BIT);

	//ColorBlend and Color Attachment
	VkPipelineColorBlendAttachmentState ColorBlendAttachment{};
	ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachment.blendEnable = false;

	VkPipelineColorBlendStateCreateInfo ColorBlending{};
	ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlending.logicOpEnable = VK_FALSE;
	ColorBlending.logicOp = VK_LOGIC_OP_COPY;
	ColorBlending.attachmentCount = 1;
	ColorBlending.pAttachments = &ColorBlendAttachment;
	ColorBlending.blendConstants[0] = 0.0;
	ColorBlending.blendConstants[1] = 0.0;
	ColorBlending.blendConstants[2] = 0.0;
	ColorBlending.blendConstants[3] = 0.0;

	//Pipeline Layout
	PX_RENDER_GENERATE_PIPELINE_LAYOUT_INFO(PipelineLayoutInfo, 0, 0);

	PX_ENSURE_RET_VOID(vkCreatePipelineLayout(device, &PipelineLayoutInfo, nullptr, &pipelineLayout))
}

QueueFamilyIndics RenderCore::FindQueueFamilies(VkPhysicalDevice device) const
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

SwapChainSupportDetails RenderCore::QuerySwapChainSupport(VkPhysicalDevice device) const
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if(formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if(presentModeCount!=0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}
