#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "Orange/Scene/Components.h"

namespace Orange
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		o_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		/*o_Context->o_Registry.each([&](auto entityID)
			{
				Entity entity{ entt , o_Context.get() };
				DrawEntityNode(entity);
			});*/

		// entt::base_registry 丢弃了 each方法 但是 skypjack 说可以像使用组件那样使用视图
		// 来自于 https://github.com/skypjack/entt/issues/1116
		for (auto& entt : o_Context->o_Registry.view<entt::entity>())
		{
			Entity entity{ entt , o_Context.get() };
			DrawEntityNode(entity);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((o_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			o_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817222, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

}
