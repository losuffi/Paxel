#include "SandBox.h"
#include "stdio.h"
#include "stdlib.h"


SandBox::SandBox()
{

}


SandBox::~SandBox()
{
}

void SandBox::Run()
{
	printf("Engine Running");
	Log::Init();
	Log::GetClientLogger()->info("Log Message!");
	system("pause");
}

void SandRunner::Update()
{

}
