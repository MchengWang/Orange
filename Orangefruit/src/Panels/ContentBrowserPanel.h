#pragma once

#include <filesystem>

#include "Orange/Renderer/Texture.h"

namespace Orange
{

	class ContentBrowerPanel
	{
	public:
		ContentBrowerPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path o_CurrentDirectory;

		Ref<Texture2D> o_DirectoryIcon;
		Ref<Texture2D> o_FileIcon;
	};

}
