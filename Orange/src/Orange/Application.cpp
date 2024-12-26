#include "ogpch.h"

#include "Application.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Log.h"

#include <GLFW/glfw3.h>

namespace Orange
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		o_Window = std::unique_ptr<Window>(Window::Create());
		o_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		OG_CORE_TRACE("{0}", e.ToString());
	}

	void Application::Run()
	{
		while (o_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			o_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		o_Running = false;
		return true;
	}
}