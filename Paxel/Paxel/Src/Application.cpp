#include "PXPCH.h"
#include "Application.h"
#ifdef PX_BUILD_DLL
std::map<DispatchName, EventDispatcher::DispathFunc> EventDispatcher::items;
#endif
#define PX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
Application::Application()
{
	origin.reset(new Window(640, 480, "App"));
	origin->SetEventCallback(PX_BIND_EVENT_FN(Application::OnEvent));
	m_ImGuiLayer = new ImguiLayer();
	
}


Application::~Application()
{

}

void Application::Run()
{	
	Log::Init();
	Log::GetCoreLogger()->info("App Start!");
	
	origin->OnInit();
	isRunning = true;
	while (isRunning)
	{
	}
}

void Application::OnEvent(Event& e)
{
}

void Application::PushLayer(Layer* layer)
{
}

void Application::PushOverlay(Layer* layer)
{
}

bool Application::isRunning = true;

