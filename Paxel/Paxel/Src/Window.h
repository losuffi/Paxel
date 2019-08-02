#pragma once
#include "PXPCH.h"
#include "RenderCore.h"
#include <GLFW/glfw3.h>
class PAXEL_API Window
{
public:
	Window(int width,int height, char* title);
	virtual void OnUpdate();
	virtual void OnInit();
	virtual void OnDestroy();
	using EventCallbackFn = std::function<void(Event&)>;
	inline void SetEventCallback(const EventCallbackFn& callback) { windowData.EventCallback = callback; }
	~Window();
protected:
	GLFWwindow* origin;
	RenderCore* renderer;
	struct WindowData
	{
		EventCallbackFn EventCallback;
	};
	WindowData windowData;
};


