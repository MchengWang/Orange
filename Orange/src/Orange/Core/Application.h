#pragma once

#include "Orange/Core/Core.h"

#include "Orange/Core/Window.h"

#include "Orange/Core/LayerStack.h"
#include "Orange/Events/Event.h"
#include "Orange/Events/ApplicationEvent.h"

#include "Orange/Core/Timestep.h"

#include "Orange/ImGui/ImGuiLayer.h"

namespace Orange
{
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);

		inline Window& GetWindow() { return *o_Window; }

		inline static Application& Get() { return *o_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event); // 窗口关闭事件
		bool OnWindowResized(WindowResizeEvent& event); // 窗口尺寸修改事件

	private:
		Scope<Window> o_Window;
		ImGuiLayer* o_ImGuiLayer;
		bool o_Running = true;
		bool o_Minimized = false; // 设置窗口最小化标志
		LayerStack o_LayerStack;

		float o_LastFrameTime = 0.0f;

	private:
		static Application* o_Instance;
	};

	// 在客户端定义
	Application* CreateApplication();
}