#include "ogpch.h"

#include "Application.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Orange
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::o_Instance = nullptr;

	Application::Application()
	{
		OG_CORE_ASSERT(!o_Instance, "Application 实例已经存在！")
		o_Instance = this;

		o_Window = std::unique_ptr<Window>(Window::Create());
		o_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		o_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(o_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		o_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* overLayer)
	{
		o_LayerStack.PushOverlay(overLayer);
		overLayer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = o_LayerStack.end(); it != o_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (o_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : o_LayerStack)
				layer->OnUpdate();

			o_ImGuiLayer->Begin();
			for (Layer* layer : o_LayerStack)
				layer->OnImGuiRender();
			o_ImGuiLayer->End();

			o_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		o_Running = false;
		return true;
	}
}