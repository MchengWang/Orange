#pragma once

#include "Core.h"

namespace Orange
{
	class ORANGE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// 在客户端定义
	Application* CreateApplication();
}