#include <Orange.h>

class ExampleLayer : public Orange::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{
	}

	void OnUpdate() override
	{
		OG_CLIENT_INFO("Ê¾Àý²ã::¸üÐÂ");
	}

	void OnEvent(Orange::Event& event) override
	{
		OG_CLIENT_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Orange::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Orange::Application* Orange::CreateApplication()
{
	return new Sandbox();
}