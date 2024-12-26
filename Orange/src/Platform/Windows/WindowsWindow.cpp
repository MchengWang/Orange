#include "ogpch.h"
#include "WindowsWindow.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Events/MouseEvent.h"
#include "Orange/Events/KeyEvent.h"

#include <glad/glad.h>

namespace Orange
{
	static bool o_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		OG_CORE_ERROR("GLFW 错误: ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		o_Data.Title = props.Title;
		o_Data.Width = props.Width;
		o_Data.Height = props.Height;

		OG_CORE_INFO("创建窗口 标题: {0}, 宽度: {1}, 高度: {2}", props.Title, props.Width, props.Height);

		if (!o_GLFWInitialized)
		{
			// TODO glfw在系统关闭时终止
			int success = glfwInit();
			OG_CORE_ASSERT(success, "不能初始化 GLFW");

			glfwSetErrorCallback(GLFWErrorCallback);
			o_GLFWInitialized = true;
		}

		o_Window = glfwCreateWindow((int)props.Width, (int)props.Height, o_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(o_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OG_CORE_ASSERT(status, "Glad 初始化失败！");

		glfwSetWindowUserPointer(o_Window, &o_Data);
		SetVSync(true);

		// 设置 GLFW 回调函数
		glfwSetWindowSizeCallback(o_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(o_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(o_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetMouseButtonCallback(o_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(o_Window, [](GLFWwindow* window, double xOff, double yOff)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOff, (float)yOff);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(o_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(o_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(o_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		o_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return o_Data.VSync;
	}
}