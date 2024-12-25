#include "ogpch.h"
#include "WindowsWindow.h"

namespace Orange
{
	static bool o_GLFWInitialized = false;

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

			o_GLFWInitialized = true;
		}

		o_Window = glfwCreateWindow((int)props.Width, (int)props.Height, o_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(o_Window);
		glfwSetWindowUserPointer(o_Window, &o_Data);
		SetVSync(true);
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