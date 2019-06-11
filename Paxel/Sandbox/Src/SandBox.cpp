#include "SandBox.h"
#include "stdio.h"
#include "stdlib.h"
#include <thread>
#include <chrono>
SandBox::SandBox()
{

}


SandBox::~SandBox()
{
}

void SandBox::Run()
{
	Log::Init();
	Log::GetClientLogger()->info("Sandbox Start!");
	GameBox* runner = new SandRunner();
	isRunning = true;
	runner->Initialize();
	while (isRunning)
	{
		runner->Update();
	}
	runner->CleanUp();
	system("pause");
}

void SandRunner::Update()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Log::GetClientLogger()->info("Update...");
}

void SandRunner::Initialize()
{
	Log::GetClientLogger()->info("Initialize");
}

void SandRunner::CleanUp()
{
	Log::GetClientLogger()->info("Clean Up");
}
