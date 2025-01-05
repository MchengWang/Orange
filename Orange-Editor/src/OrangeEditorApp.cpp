#include <Orange.h>
#include <Orange/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Orange
{

	class OrangeEditor : public Orange::Application
	{
	public:
		OrangeEditor()
			: Application("Orange Editor")
		{
			PushLayer(new EditorLayer());
		}

		~OrangeEditor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new OrangeEditor();
	}
}