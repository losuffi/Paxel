#pragma once
#include "Paxel.h"
class SandBox: public Application
{
public:
	SandBox();
	~SandBox();
	virtual void Run() override;
};

class SandRunner : public GameBox
{
public:
	virtual void Update() override;
	virtual void Initialize() override;
	virtual void CleanUp() override;
};

Application* CreateApplication()
{
	return new SandBox();
}