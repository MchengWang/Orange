#pragma once

#include <string>
#include <optional>

namespace Orange
{

	class FileDialogs
	{
	public:
		// ���ȡ�����򷵻ؿ��ַ���
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};

}