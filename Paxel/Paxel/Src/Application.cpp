#include "PXPCH.h"
#include "Application.h"

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

} 