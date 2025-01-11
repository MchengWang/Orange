#pragma once

#include <string>
#include <optional>

namespace Orange
{

	class FileDialogs
	{
	public:
		// 如果取消，则返回空字符串
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}
