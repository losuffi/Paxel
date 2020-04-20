#include "PXPCH.h"
#include "ImguiLayer.h"
#include "imgui.h"
#include "RenderCore.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"
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
	ImGui_ImplVulkan_InitInfo init_info{ VkInfo->instance,
		VkInfo->physicalDevice,
		VkInfo->device,
		VkInfo->familyIndics.graphicsFamily.value(),
		VkInfo->graphicsQueue,
	};
	ImGui_ImplVulkan_Init("3s")
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
