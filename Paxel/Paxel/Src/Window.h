#pragma once
#include "PXPCH.h"
class PAXEL_API Window
{
public:
	Window(){};
	virtual void OnUpdate() =0;
	virtual void OnInit() = 0;
	virtual void OnDestroy() = 0;
	using EventCallbackFn = std::function<void(Event&)>;
	inline void SetEventCallback(const EventCallbackFn& callback) { windowData.EventCallback = callback; }
	static Window* Create(int width, int height, char* title);
	virtual void* GetNativeWindow() const = 0;
	virtual void* GetNativeInfoList() const = 0;
	virtual ~Window() = default;;
protected:
	struct WindowData
	{
		EventCallbackFn EventCallback;
	};
	WindowData windowData;
};


