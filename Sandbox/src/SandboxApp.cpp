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
		//OG_CLIENT_INFO("示例层::更新");
		
		if (Orange::Input::IsKeyPressed(OG_KEY_TAB))
			OG_CLIENT_TRACE("tab 键被按下 (轮询)");
	}

	void OnEvent(Orange::Event& event) override
	{
		//OG_CLIENT_TRACE("{0}", event.ToString());

		if (event.GetEventType() == Orange::EventType::KeyPressed)
		{
			Orange::KeyPressedEvent& e = (Orange::KeyPressedEvent&)event;

			if (e.GetKeyCode() == OG_KEY_TAB)
				OG_CLIENT_TRACE("tab 键被按下 (事件)");

			OG_CLIENT_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Orange::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverLayer(new Orange::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Orange::Application* Orange::CreateApplication()
{
	return new Sandbox();
}