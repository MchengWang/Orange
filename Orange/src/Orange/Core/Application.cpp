#include "ogpch.h"

#include "Application.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Log.h"

#include "Orange/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>

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

		Renderer::Init();

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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

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
			float time = (float)glfwGetTime();
			Timestep timestep = time - o_LastFrameTime;
			o_LastFrameTime = time;

			if (!o_Minimized) // 当窗口不是最小化的时候，进行图层更新
			{
				for (Layer* layer : o_LayerStack)
					layer->OnUpdate(timestep);
			}

			o_ImGuiLayer->Begin();
			for (Layer* layer : o_LayerStack)
				layer->OnImGuiRender();
			o_ImGuiLayer->End();

			o_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		o_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0) // 当窗口尺寸最小化时，宽度和高度即为0
		{
			o_Minimized = true;
			return false;
		}

		o_Minimized = false;
		Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

		return false;
	}
}