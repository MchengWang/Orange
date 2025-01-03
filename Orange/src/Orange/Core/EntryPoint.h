#pragma once
#include "Orange/Core/Core.h"

#ifdef OG_PLATFORM_WINDOWS

extern Orange::Application* Orange::CreateApplication();

int main(int argc, char** argv)
{
	Orange::Log::Init();
	OG_CORE_WARN("初始化日志！");

	HZ_PROFILE_BEGIN_SESSION("Startup", "OrangePf-Startup.json");
	auto app = Orange::CreateApplication();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "OrangePf-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "OrangePf-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif // OG_PLATFORM_WINDOWS
