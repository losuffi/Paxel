#include "PXPCH.h"
#include "Application.h"
#ifdef PX_BUILD_DLL
std::map<DispatchName, EventDispatcher::DispathFunc> EventDispatcher::items;
#endif

Application::Application()
{
}


Application::~Application()
{

}

void Application::Run()
{	
	Log::Init();
	Log::GetCoreLogger()->info("App Start!");
	origin.reset(new Window(640, 480, "App"));
	std::unique_ptr<GameBox> runner (new GameBox(origin));
	isRunning = true;
	runner->Initialize();
	while (isRunning)
	{
		runner->Update();
	}
	runner->CleanUp();
}

bool Application::isRunning = true;

void GameBox::Initialize()
{
	window->OnInit();
	window->SetEventCallback(std::bind(&GameBox::OnEvent,this,std::placeholders::_1));
	EventDispatcher::AddHandle(EVENT_DN_TEST, TestFunc);
}

void GameBox::Update()
{
	window->OnUpdate();
}

void GameBox::CleanUp()
{
	window->OnDestroy();
}

void GameBox::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::KeyPressed)
	{
		auto keyEvent = (KeyPresssedEvent*)&e;
		if (keyEvent->Getint() == 81)
		{
			Test();
		}
	}
}

void GameBox::Test()
{
	auto a = std::rand();
	auto b = std::rand();
	const char ** args;
	args = (const char **)malloc(2 * sizeof(char*));
	args[0] = (std::to_string(a)).c_str();
	args[1] = (std::to_string(b)).c_str();
	EventDispatcher::Call(EVENT_DN_TEST, args);
	free(args);
}
