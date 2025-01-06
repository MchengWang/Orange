#include <Orange.h>
#include <Orange/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Orange
{

	class Orangefruit : public Orange::Application
	{
	public:
		Orangefruit()
			: Application("Orangefruit")
		{
			PushLayer(new EditorLayer());
		}

		~Orangefruit()
		{

		}

	};

	Application* CreateApplication()
	{
		return new Orangefruit();
	}
}