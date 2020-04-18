#include "PXPCH.h"
#include "Application.h"
#define PX_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;
Application::Application()
{
	s_Instance = this;
	origin.reset(Window::Create(1080,640,"Paxel Engine"));
	origin->SetEventCallback(PX_BIND_EVENT_FN(Application::OnEvent));
	m_ImGuiLayer = new ImguiLayer();
	m_ImGuiLayer->SetNativeWindow(origin->GetNativeWindow());
	m_ImGuiLayer->SetVkRenderCoreInfoList(origin->GetNativeInfoList());
	PushLayer(m_ImGuiLayer);
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
		for(Layer* layer : m_LayerStack)
		{
			layer->OnBegin();
			layer->OnUpdate();
			layer->OnEnd();
		}
		origin->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher<WindowsCloseEvent> dispatcher(e);
	dispatcher.Dispatch(PX_BIND_EVENT_FN(OnWindowsClose));

	for(auto it = m_LayerStack.end(); it!=m_LayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if(e.Handled)
			break;
	}
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

bool Application::OnWindowsClose(WindowsCloseEvent& e)
{
	isRunning = false;
	return true;
}


bool Application::isRunning = true;

