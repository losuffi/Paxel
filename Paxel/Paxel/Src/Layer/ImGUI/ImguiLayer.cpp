#include "PXPCH.h"
#include "ImguiLayer.h"
#include "VulkanRenderCore.h"
void ImguiLayer::OnBegin()
{
}

void ImguiLayer::OnEnd()
{
}

void ImguiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	const auto window = static_cast<GLFWwindow*>(NativeWindow);
	PX_ENSURE_RET_VOID(window, "native window is nullptr");
	ImGui_ImplGlfw_InitForVulkan(window, true);

	auto VkInfo = static_cast<VkRenderCoreInfoList*>(RenderCoreInfoList);
	auto Fn = [](VkResult res)
	{
		if(res == VK_SUCCESS)
		{
			PX_CORE_INFO("Init Imgui succ!!");
			return;
		}
		PX_CORE_ERROR("Failed to init Imgui!");
	};
	ImGui_ImplVulkan_InitInfo init_info{ VkInfo->instance,
		VkInfo->physicalDevice,
		VkInfo->device,
		VkInfo->familyIndics.graphicsFamily.value(),
		VkInfo->graphicsQueue,
		VK_NULL_HANDLE,
		VK_NULL_HANDLE,
		VkInfo->ImageViewCount,
		VkInfo->ImageViewCount,
		VK_SAMPLE_COUNT_32_BIT,
		nullptr,
		Fn
	};
}

void ImguiLayer::OnDettach()
{
}

void ImguiLayer::OnEvent(Event& event)
{
}

void ImguiLayer::OnUpdate()
{
}

void ImguiLayer::SetVkRenderCoreInfoList(void* VkInfoList)
{
	RenderCoreInfoList = &VkInfoList;
}
