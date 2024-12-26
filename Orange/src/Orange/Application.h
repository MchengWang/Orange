#pragma once

#include "Core.h"

#include "Events/Event.h"

#include "Orange/Events/ApplicationEvent.h"

#include "Window.h"

namespace Orange
{
	class ORANGE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> o_Window;
		bool o_Running = true;
	};

	// 在客户端定义
	Application* CreateApplication();
}