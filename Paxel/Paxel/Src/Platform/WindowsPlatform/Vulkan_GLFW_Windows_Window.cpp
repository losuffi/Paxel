#include "PXPCH.h"
#include "Vulkan_GLFW_Windows_Window.h"
Window* Window::Create(int width, int height, char* title)
{
	return new Vulkan_GLFW_Windows_Window(width, height, title);
}

Vulkan_GLFW_Windows_Window::Vulkan_GLFW_Windows_Window(int width, int height, char* title) :renderer {nullptr}
{
	if (!glfwInit())
	{
		Log::GetCoreLogger()->error("failed window done!");
		return;
	}
	origin = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Vulkan_GLFW_Windows_Window::~Vulkan_GLFW_Windows_Window()
{
	glfwDestroyWindow(origin);
	glfwTerminate();
}

void Vulkan_GLFW_Windows_Window::OnUpdate()
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(origin);
	glfwPollEvents();
}

void Vulkan_GLFW_Windows_Window::OnInit()
{
	glfwSetWindowUserPointer(origin, &windowData);
	renderer = new VulkanRenderCore();
	renderer->SetupVulkan();
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

void Vulkan_GLFW_Windows_Window::OnDestroy()
{
	renderer->OnDestroy();
}

void* Vulkan_GLFW_Windows_Window::GetNativeWindow() const
{
	return origin;
}

void* Vulkan_GLFW_Windows_Window::GetNativeInfoList() const
{
	auto InfoList = renderer->GetInfoList();
	return &InfoList;
}
