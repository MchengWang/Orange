#include <Orange.h>
#include <Orange/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Orange
{

	class Orangefruit : public Orange::Application
	{
	public:
		Orangefruit(ApplicationCommandLineArgs args)
			: Application("Orangefruit", args)
		{
			PushLayer(new EditorLayer());
		}

		~Orangefruit()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Orangefruit(args);
	}
}
