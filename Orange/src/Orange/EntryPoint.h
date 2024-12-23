#pragma once

#ifdef OG_PLATFORM_WINDOWS

extern Orange::Application* Orange::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Orange::CreateApplication();
	app->Run();
	delete app;
}

#endif // OG_PLATFORM_WINDOWS
