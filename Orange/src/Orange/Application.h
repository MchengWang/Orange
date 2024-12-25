#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Window.h"

namespace Orange
{
	class ORANGE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> o_Window;
		bool o_Running = true;
	};

	// 在客户端定义
	Application* CreateApplication();
}