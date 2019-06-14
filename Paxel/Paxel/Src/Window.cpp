#pragma once
#include "Window.h"
#include "Log.h"
Window::Window(int width, int height, char* title)
{
	if (!glfwInit())
	{
		Log::GetCoreLogger()->error("failed window done!");
		return;
	}
	origin = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Window::~Window()
{
	glfwTerminate();
}
