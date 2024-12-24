#include "Application.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Log.h"

namespace Orange
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			OG_CLIENT_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			OG_CLIENT_TRACE(e.ToString());
		}

		while (true);
	}
}