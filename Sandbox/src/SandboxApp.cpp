#include <Orange.h>
#include <Orange/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "ExampleLayer.h"

class Sandbox : public Orange::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Orange::Application* Orange::CreateApplication()
{
	return new Sandbox();
}