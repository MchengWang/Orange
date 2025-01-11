#pragma once

#include <filesystem>

namespace Orange
{

	class ContentBrowerPanel
	{
	public:
		ContentBrowerPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path o_CurrentDirectory;
	};

}
