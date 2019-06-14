#pragma once
#include "Core.h"
#include <GLFW/glfw3.h>
class PAXEL_API Window
{
public:
	Window(int width,int height, char* title);
	~Window();
protected:
	GLFWwindow* origin;
};


