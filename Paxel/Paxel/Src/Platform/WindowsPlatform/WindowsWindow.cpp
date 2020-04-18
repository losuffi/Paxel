#include "PXPCH.h"
#include "WindowsWindow.h"
Window* Window::Create(int width, int height, char* title)
{
	return new WindowsWindow(width, height, title);
}

WindowsWindow::WindowsWindow(int width, int height, char* title) :renderer {nullptr}
{
	if (!glfwInit())
	{
		Log::GetCoreLogger()->error("failed window done!");
		return;
	}
	origin = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(origin);
	glfwTerminate();
}

void WindowsWindow::OnUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(origin);
	glfwPollEvents();
}

void WindowsWindow::OnInit()
{
	glfwSetWindowUserPointer(origin, &windowData);
	renderer = new RenderCore();
	renderer->OnInit(origin);
	glfwSetKeyCallback(origin, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		const auto m_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		switch (action)
		{
		case GLFW_PRESS:
		{
			Log::GetClientLogger()->info(key);
			KeyPresssedEvent e(key);
			m_data.EventCallback(e);
			break;
		}
		default:
			break;
		}
	});
	glfwSetWindowCloseCallback(origin, [](GLFWwindow* window)
	{
		const auto m_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowsCloseEvent e;
		m_data.EventCallback(e);
	});
}

void WindowsWindow::OnDestroy()
{
	renderer->OnDestroy();
}

void* WindowsWindow::GetNativeWindow() const
{
	return origin;
}

void* WindowsWindow::GetNativeInfoList() const
{
	auto InfoList = renderer->GetInfoList();
	return &InfoList;
}
