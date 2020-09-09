#pragma once
#include "Window.h"
#include "RenderCore.h"
#include <GLFW/glfw3.h>
class WindowsWindow : public Window
{
public:
	WindowsWindow(int width, int height, char* title);
	~WindowsWindow();
	void OnUpdate() override;
	void OnInit() override;
	void OnDestroy() override;

	void* GetNativeWindow() const override;

	void* GetNativeInfoList() const override;
protected:
	GLFWwindow* origin;
	RenderCore* renderer;
};

