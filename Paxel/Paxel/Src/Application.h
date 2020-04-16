#pragma once
#include "Core.h"
#include "Window.h"
#include "Layer/ImGUI/ImguiLayer.h"
class PAXEL_API Application
{
public:
	Application();
 	virtual ~Application();
	virtual void Run();
	void OnEvent(Event& e);
	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
public:
	static bool isRunning;
protected:
	std::shared_ptr<Window> origin;
	LayerStack m_LayerStack;
	ImguiLayer* m_ImGuiLayer;
};


Application* CreateApplication();
