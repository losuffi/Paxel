#include "PXPCH.h"
#include "VulkanRenderCore.h"

//#include <examples/imgui_impl_glfw.h>

#include "Utils/FileUtils.h"
void VulkanRenderCore::OnDestroy()
{
	for(auto imageView : swapchainImageviews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	
	vkDestroySurfaceKHR(instance, surface, nullptr);
	for(auto buffer : swapchainFramebuffers)
	{
		vkDestroyFramebuffer(device, buffer, nullptr);
	}
	vkDestroyCommandPool(device, CommandPool, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance,nullptr);
}

void VulkanRenderCore::ImguiRendererBinding(GLFWwindow* window)
{
	//ImGui_ImplGlfw_InitForVulkan(window, true);
	//ImGui_ImplVulkan_InitInfo initInfo = {};
	//initInfo.Instance = instance;
	//initInfo.PhysicalDevice = PhysicalDevice;
	//initInfo.Device = device;
	//initInfo.QueueFamily = FindQueueFamilies(PhysicalDevice).graphicsFamily.value();
	//initInfo.Queue = graphicsQueue;
	//initInfo.PipelineCache = nullptr;
	//initInfo.Allocator = nullptr;
	//initInfo.MinImageCount = PX_MIN_IMAGE_COUNT;
	//initInfo.ImageCount = WindowData.ImageCount;
	//initInfo.CheckVkResultFn = nullptr;
	//ImGui_ImplVulkan_Init(&initInfo, WindowData.RenderPass);
}

VkRenderCoreInfoList VulkanRenderCore::GetInfoList() const
{
	const auto indics = FindQueueFamilies(PhysicalDevice);
	return VkRenderCoreInfoList{
		instance, PhysicalDevice, device,
		graphicsQueue, presentQueue, surface,
		indics, static_cast<uint32_t>(swapchainImageviews.size())};
}

void VulkanRenderCore::CreateVkInstance()
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

void VulkanRenderCore::CheckVkExtensions()
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

void VulkanRenderCore::PickPhysicalDevice()
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

void VulkanRenderCore::CreateLogicDevice()
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

bool VulkanRenderCore::isSuitableDevice(VkPhysicalDevice device)
{
	QueueFamilyIndics indices = FindQueueFamilies(device);
	return indices.IsComplete();
}

bool VulkanRenderCore::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	return false;
}



void VulkanRenderCore::CreateSwapChain()
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
	CreateRenderPass(SwapChainSupport.formats[0].format);
}

void VulkanRenderCore::CreateImageViews(VkFormat ImageViewFormat)
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

VkShaderModule VulkanRenderCore::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	CreateInfo.codeSize = code.size();
	CreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule ShaderModule;
	PX_ENSURE_RET_VAL(vkCreateShaderModule(device, &CreateInfo, nullptr, &ShaderModule) == VK_SUCCESS, nullptr, "Failed to create shader module!");
	return ShaderModule;
}

void VulkanRenderCore::CreateGraphicPipeline(const std::string& VertShaderFile,const std::string& FragShaderFile)
{
	//Shader Specify
	const VkShaderModule VertShaderModule = CreateShaderModule(FileUtils::ReadFile(VertShaderFile));
	const VkShaderModule FragShaderModule = CreateShaderModule(FileUtils::ReadFile(FragShaderFile));
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
	PX_ENSURE_RET_VOID(vkCreatePipelineLayout(device, &PipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, "Failed to create pipeline layout!");

	//Pipeline Create
	VkGraphicsPipelineCreateInfo PipelineInfo{};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = 2;
	PipelineInfo.pStages = ShaderStages;
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &InputAssembly;
	PipelineInfo.pViewportState = &ViewportState;
	PipelineInfo.pRasterizationState = &Rasterization;
	PipelineInfo.pMultisampleState = &MultiSampling;
	PipelineInfo.pColorBlendState = &ColorBlending;
	PipelineInfo.layout = pipelineLayout;
	PipelineInfo.renderPass = renderPass;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	PipelineInfo.basePipelineIndex = -1;
	PX_ENSURE_RET_VOID(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &graphicPipeline) == VK_SUCCESS, 
		"Failed to create graphics pipeline!");
}

void VulkanRenderCore::CreateFramebuffers()
{
	swapchainFramebuffers.resize(swapchainImageviews.size());
	for(size_t i = 0; i < swapchainImageviews.size(); ++ i)
	{
		VkImageView Attachments[] = { swapchainImageviews[i] };
		VkFramebufferCreateInfo FramebufferInfo{};
		FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferInfo.renderPass = renderPass;
		FramebufferInfo.pAttachments = Attachments;
		FramebufferInfo.attachmentCount = 1;
		FramebufferInfo.width = swapchainExtent.width;
		FramebufferInfo.height = swapchainExtent.height;
		FramebufferInfo.layers = 1;
		PX_ENSURE_RET_VOID(vkCreateFramebuffer(device, &FramebufferInfo, nullptr, &swapchainFramebuffers[i]) == VK_SUCCESS, "Failed to create framebuffer!")
	}
}

void VulkanRenderCore::CreateRenderPass(VkFormat ViewFormat)
{
	// A render pass represents a collection of attachments, subpasses, and dependencies between
	//the subpasses, and describes how the attachments are used over the course of the subpasses.
	VkAttachmentDescription ColorAttachment{};
	ColorAttachment.format = ViewFormat;
	ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference ColorAttachmentRef{};
	ColorAttachmentRef.attachment = 0;
	ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription SubPass{};
	SubPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	SubPass.colorAttachmentCount = 1;
	SubPass.pColorAttachments = &ColorAttachmentRef;

	VkRenderPassCreateInfo RenderPassInfo{};
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.attachmentCount = 1;
	RenderPassInfo.pAttachments = &ColorAttachment;
	RenderPassInfo.subpassCount = 1;
	RenderPassInfo.pSubpasses = &SubPass;

	PX_ENSURE_RET_VOID(vkCreateRenderPass(device, &RenderPassInfo, nullptr, &renderPass) == VK_SUCCESS, "failed to create render pass!");
}

void VulkanRenderCore::CreateCommandPool()
{
	QueueFamilyIndics QueueFamilyIndices = FindQueueFamilies(PhysicalDevice);
	VkCommandPoolCreateInfo PoolInfo{};
	PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	PoolInfo.queueFamilyIndex = QueueFamilyIndices.graphicsFamily.value();
	PoolInfo.flags = 0;
	PX_ENSURE_RET_VOID(vkCreateCommandPool(device, &PoolInfo, nullptr, &CommandPool) == VK_SUCCESS, "failed to create command pool!");
}

void VulkanRenderCore::CreateCommandBuffers()
{
	CommandBuffers.resize(swapchainFramebuffers.size());
	VkCommandBufferAllocateInfo AllocInfo{};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.commandPool = CommandPool;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandBufferCount = static_cast<uint32_t>(CommandBuffers.size());

	PX_ENSURE_RET_VOID(vkAllocateCommandBuffers(device, &AllocInfo, CommandBuffers.data()) == VK_SUCCESS, "failed to allocate command buffers!");

	for(size_t i = 0; i < CommandBuffers.size(); ++i)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		PX_ENSURE_RET_VOID(vkBeginCommandBuffer(CommandBuffers[i], &beginInfo) == VK_SUCCESS, "failed to begin recording command buffer!");

		VkRenderPassBeginInfo RenderPassInfo{};
		RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassInfo.renderPass = renderPass;
		RenderPassInfo.framebuffer = swapchainFramebuffers[i];
		RenderPassInfo.renderArea.offset = {0, 0};
		RenderPassInfo.renderArea.extent = swapchainExtent;

		VkClearValue ClearColor = {0.0f, 0.0f, 0.0f, 0.0f};
		RenderPassInfo.clearValueCount = 1;
		RenderPassInfo.pClearValues = &ClearColor;

		vkCmdBeginRenderPass(CommandBuffers[i], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);
		vkCmdDraw(CommandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(CommandBuffers[i]);
		PX_ENSURE_RET_VOID(vkEndCommandBuffer(CommandBuffers[i]) == VK_SUCCESS, "failed to record command buffer");
	}
}

void VulkanRenderCore::CreateDescriptorPool()
{
	VkDescriptorPoolSize poolSize[] =
	{
		{VK_DESCRIPTOR_TYPE_SAMPLER,	1000},
		{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,	1000},
		{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	1000},
		{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	1000},
		{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,	1000},
		{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,	1000},
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	1000},
		{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,	1000},
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,	1000},
		{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,	1000},
		{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,	1000}
	};
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType	=	VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags	=	VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets	=	1000	*	static_cast<int>(sizeof(poolSize) / sizeof(*poolSize));
	poolInfo.poolSizeCount	=	static_cast<uint32_t>(sizeof(poolSize)/sizeof(*poolSize));
	poolInfo.pPoolSizes		=	poolSize;
	PX_ENSURE_RET_VOID(vkCreateDescriptorPool(device,&poolInfo,nullptr,&descriptorPool) 
		== VK_SUCCESS,"failed to create descriptor pool");
	
}

void VulkanRenderCore::SetupVulkan()
{
	//CreateVkInstance();
	//PickPhysicalDevice();
	//CreateLogicDevice();
	//CreateDescriptorPool();
}

void VulkanRenderCore::SetVulkanWindow(ImGui_ImplVulkanH_Window* window, VkSurfaceKHR surface, int width, int height)
{
	window->Surface = surface;
	VkBool32 res;
	uint32_t queueIndex = FindQueueFamilies(PhysicalDevice).graphicsFamily.value();
	vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, queueIndex, window->Surface, &res);
	PX_ENSURE_RET_VOID(res,"Error no WSI support on physical device");

	const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM,VK_FORMAT_B8G8R8_UNORM,VK_FORMAT_R8G8B8_UNORM};
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	window->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(PhysicalDevice, window->Surface,
		requestSurfaceImageFormat,IM_ARRAYSIZE(requestSurfaceImageFormat),requestSurfaceColorSpace);

	VkPresentModeKHR presentModes[] = {VK_PRESENT_MODE_FIFO_KHR};
	window->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(PhysicalDevice, window->Surface,
		&presentModes[0], IM_ARRAYSIZE(presentModes));

	ImGui_ImplVulkanH_CreateWindow(instance,PhysicalDevice,device,window,queueIndex,
		nullptr,width,height, PX_MIN_IMAGE_COUNT);
}

void VulkanRenderCore::CleanupVulkanWindow()
{
	ImGui_ImplVulkanH_DestroyWindow(instance,device,&WindowData,nullptr);
}

void VulkanRenderCore::CreateVulkanInstance(VkInstance& Instance)
{
	CheckVkExtensions();
	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pApplicationName = "Paxel Engine";
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
	
#ifdef PAXEL_LOW_LEVEL_RUNTIME_LOG_ENABLE
	Log::GetCoreLogger()->info("glfw require extensions:");
	for (int i =0; i < glfwExtensionCount; i++)
	{
		Log::GetCoreLogger()->info(*(glfwExtension + i));
	}
#endif

	createinfo.enabledExtensionCount = glfwExtensionCount;
	createinfo.ppEnabledExtensionNames = glfwExtension;
	createinfo.enabledLayerCount = 0;
	
	PX_ENSURE_RET_VOID(vkCreateInstance(&createinfo, nullptr, &Instance) == VK_SUCCESS,"Create intance failed!");
}

QueueFamilyIndics VulkanRenderCore::FindQueueFamilies(VkPhysicalDevice device) const
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

SwapChainSupportDetails VulkanRenderCore::QuerySwapChainSupport(VkPhysicalDevice device) const
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