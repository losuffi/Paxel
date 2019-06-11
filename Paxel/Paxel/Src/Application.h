#pragma once
#include "Core.h"
class PAXEL_API Application
{
public:
	Application();
 	virtual ~Application();
	virtual void Run() = 0;
};

class PAXEL_API GameBox
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void CleanUp() = 0;
};

Application* CreateApplication();
