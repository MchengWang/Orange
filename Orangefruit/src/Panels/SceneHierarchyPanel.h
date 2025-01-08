#pragma once

#include "Orange/Core/Base.h"
#include "Orange/Core/Log.h"
#include "Orange/Scene/Scene.h"
#include "Orange/Scene/Entity.h"

namespace Orange
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> o_Context;
		Entity o_SelectionContext;
	};

}
