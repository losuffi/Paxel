#pragma once
#include "Core.h"
class PAXEL_API Application
{
public:
	Application();
 	virtual ~Application();
	virtual void Run() = 0;
};
Application* CreateApplication();
