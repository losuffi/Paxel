#pragma once
#include "Paxel.h"
class SandBox: public Application
{
public:
	SandBox();
	~SandBox();
	virtual void Run() override;
};

Application* CreateApplication()
{
	return new SandBox();
}