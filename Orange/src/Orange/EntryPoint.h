#pragma once

#ifdef OG_PLATFORM_WINDOWS

extern Orange::Application* Orange::CreateApplication();

int main(int argc, char** argv)
{
	Orange::Log::Init();
	OG_CORE_WARN("��ʼ����־��");

	auto app = Orange::CreateApplication();
	app->Run();
	delete app;
}

#endif // OG_PLATFORM_WINDOWS
