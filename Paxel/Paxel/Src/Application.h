#pragma once
#include "Core.h"
#include "Window.h"
class PAXEL_API Application
{
public:
	Application();
 	virtual ~Application();
	virtual void Run();
public:
	static bool isRunning;
protected:
	std::shared_ptr<Window> origin;
};

class PAXEL_API GameBox
{
public:
	std::shared_ptr<Window> window;
	GameBox(std::shared_ptr<Window> win) :window(win) {};
	virtual void Initialize();
	virtual void Update();
	virtual void CleanUp();
	virtual void OnEvent(Event& e);
};

Application* CreateApplication();
