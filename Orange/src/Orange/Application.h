#pragma once

#include "Core.h"

#include "Window.h"

#include "Orange/LayerStack.h"
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
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> o_Window;
		ImGuiLayer* o_ImGuiLayer;
		bool o_Running = true;
		LayerStack o_LayerStack;

		float o_LastFrameTime = 0.0f;

	private:
		static Application* o_Instance;
	};

	// 在客户端定义
	Application* CreateApplication();
}