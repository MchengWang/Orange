#include "ogpch.h"

#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Orange
{

	// Once we hace projects, change this
	static const std::filesystem::path o_AssetPath = "assets";

	ContentBrowerPanel::ContentBrowerPanel()
		: o_CurrentDirectory(o_AssetPath)
	{
	}

	void ContentBrowerPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (o_CurrentDirectory != std::filesystem::path(o_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				o_CurrentDirectory = o_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntity : std::filesystem::directory_iterator(o_CurrentDirectory))
		{
			const auto& path = directoryEntity.path();
			auto relativePath = std::filesystem::relative(path, o_AssetPath);
			std::string filenameString = relativePath.filename().string();
			if (directoryEntity.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					o_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}

}
