#pragma once
#include "Core.h"
#include "PXPCH.h"
#include "vulkan/vulkan.h"
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
	void CreateVkInstance();
	void CheckVkExtensions();
	void PickPhysicalDevice();
	void CreateLogicDevice();
	bool isSuitableDevice(VkPhysicalDevice device);
protected:
	GLFWwindow* origin;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	struct WindowData
	{
		EventCallbackFn EventCallback;
	};
	WindowData windowData;
};


