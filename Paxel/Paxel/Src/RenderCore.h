#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include "PXPCH.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <examples/imgui_impl_vulkan.h>
#include "Core.h"

#define PX_MIN_IMAGE_COUNT 2

#define PX_RENDER_GENERATE_SHADER_STAGE_INFO(InfoName, Stage, ShaderModule, EnterFunc)\
	VkPipelineShaderStageCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;\
	InfoName.stage = Stage;\
	InfoName.module = ShaderModule;\
	InfoName.pName = #EnterFunc;

#define PX_RENDER_GENERATE_VERTEX_INPUT_STATE_INFO(InfoName, VertexBindingDescriptionCount, VertexAttributeDescriptionCount)\
	VkPipelineVertexInputStateCreateInfo InfoName{}; \
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;\
	InfoName.vertexBindingDescriptionCount = VertexBindingDescriptionCount;\
	InfoName.vertexAttributeDescriptionCount = VertexAttributeDescriptionCount;

#define PX_RENDER_GENERATE_INPUT_ASSEMBLY_STATE_INFO(InfoName, Topology, PrimitiveRestartEnable)\
	VkPipelineInputAssemblyStateCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;\
	InfoName.topology = Topology;\
	InfoName.primitiveRestartEnable = PrimitiveRestartEnable;

#define PX_RENDER_GENERATE_VIEWPORT_STATE_INFO(InfoName, ViewportCount, ViewportArrPTR, ScissorCount, ScissorArrPTR)\
	VkPipelineViewportStateCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;\
	InfoName.viewportCount = ViewportCount;\
	InfoName.pViewports = ViewportArrPTR;\
	InfoName.scissorCount = ScissorCount;\
	InfoName.pScissors = ScissorArrPTR;

#define PX_RENDER_GENERATE_RASTERIZATION_STATE_INFO(InfoName, DepthClampEnable, RasterizationDiscardEnable, PolygonMode, LineWidth, CullMode, FrontFace, DepthBiasEnable)\
	VkPipelineRasterizationStateCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;\
	InfoName.depthClampEnable = DepthClampEnable;\
	InfoName.rasterizerDiscardEnable = RasterizationDiscardEnable;\
	InfoName.polygonMode = PolygonMode;\
	InfoName.lineWidth = LineWidth;\
	InfoName.cullMode = CullMode;\
	InfoName.frontFace = FrontFace;\
	InfoName.depthBiasEnable = DepthBiasEnable;

#define PX_RENDER_GENERATE_MULTI_SAMPLE_STATE_INFO(InfoName, SampleShaderEnable, RasterizationSamples)\
	VkPipelineMultisampleStateCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;\
	InfoName.sampleShadingEnable = SampleShaderEnable;\
	InfoName.rasterizationSamples = RasterizationSamples;

#define PX_RENDER_GENERATE_PIPELINE_LAYOUT_INFO(InfoName, LayoutCount, PushConstantRangeCount)\
	VkPipelineLayoutCreateInfo InfoName{};\
	InfoName.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;\
	InfoName.setLayoutCount = LayoutCount;\
	InfoName.pushConstantRangeCount = PushConstantRangeCount;

struct PAXEL_API QueueFamilyIndics
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct PAXEL_API SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct PAXEL_API VkRenderCoreInfoList
{
	const VkInstance instance;
	const VkPhysicalDevice physicalDevice;
	const VkDevice device;
	const VkQueue graphicsQueue;
	const VkQueue presentQueue;
	const VkSurfaceKHR surface;
	const QueueFamilyIndics familyIndics;
	const uint32_t ImageViewCount;
};

class PAXEL_API RenderCore
{
public:
	void SetupVulkan();
	void SetVulkanWindow(ImGui_ImplVulkanH_Window* window, VkSurfaceKHR surface,int	width,	int	height);
	void CleanupVulkanWindow();
	void OnDestroy();
	void ImguiRendererBinding(GLFWwindow* window);
	[[nodiscard]] VkRenderCoreInfoList GetInfoList() const;
public:
	ImGui_ImplVulkanH_Window WindowData;
protected:
	void CreateVkInstance();
	void PickPhysicalDevice();
	void CreateLogicDevice();
	bool isSuitableDevice(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateSwapChain();
	void CreateImageViews(VkFormat ImageViewFormat);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateGraphicPipeline(const std::string& VertShaderFile, const std::string& FragShaderFile);
	void CreateFramebuffers();
	void CreateRenderPass(VkFormat ViewFormat);
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateDescriptorPool();

	
	
	//Function Collections
	static void CreateVulkanInstance(VkInstance& Instance);
	static void CheckVkExtensions();
	
	QueueFamilyIndics FindQueueFamilies(VkPhysicalDevice device) const;
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
protected:
	//GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice PhysicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkExtent2D swapchainExtent;
	VkSwapchainKHR swapchain;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicPipeline;
	VkRenderPass renderPass;
	VkCommandPool CommandPool;
	VkDescriptorPool descriptorPool;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageviews;
	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> CommandBuffers;
};

