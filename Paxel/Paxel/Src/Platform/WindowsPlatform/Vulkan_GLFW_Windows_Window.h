#pragma once
#include "Window.h"
#include "VulkanRenderCore.h"
#include <GLFW/glfw3.h>
class Vulkan_GLFW_Windows_Window : public Window
{
public:
	Vulkan_GLFW_Windows_Window(int width, int height, char* title);
	~Vulkan_GLFW_Windows_Window();
	void OnUpdate() override;
	void OnInit() override;
	void OnDestroy() override;

	void* GetNativeWindow() const override;

	void* GetNativeInfoList() const override;
protected:
	GLFWwindow* origin;
	VulkanRenderCore* renderer;
};

