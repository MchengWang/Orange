#include <Orange.h>

#include "imgui/imgui.h"

class ExampleLayer : public Orange::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Orange::Input::IsKeyPressed(OG_KEY_TAB))
			OG_CLIENT_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Orange::Event& event) override
	{
		if (event.GetEventType() == Orange::EventType::KeyPressed)
		{
			Orange::KeyPressedEvent& e = (Orange::KeyPressedEvent&)event;
			if (e.GetKeyCode() == OG_KEY_TAB)
				OG_CLIENT_TRACE("Tab key is pressed (event)!");
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
	}

	~Sandbox()
	{

	}

};

Orange::Application* Orange::CreateApplication()
{
	return new Sandbox();
}