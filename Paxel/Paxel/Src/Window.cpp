#pragma once
#include "PXPCH.h"
#include "Window.h"
Window::Window(int width, int height, char* title)
{
	if (!glfwInit())
	{
		Log::GetCoreLogger()->error("failed window done!");
		return;
	}
	origin = glfwCreateWindow(width, height, title, nullptr, nullptr);
}


void Window::OnUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(origin);
	glfwPollEvents();
}

void Window::OnInit()
{
	glfwSetWindowUserPointer(origin, &windowData);
	glfwSetKeyCallback(origin, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto m_data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS:
		{
			Log::GetClientLogger()->info(key);
			KeyPresssedEvent event(key);
			m_data.EventCallback(event);
			break;
		}
		default:
			break;
		}
	});
}

void Window::OnDestroy()
{

}

Window::~Window()
{
	glfwTerminate();
}
