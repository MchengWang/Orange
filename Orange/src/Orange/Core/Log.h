#pragma once

#include "Orange/Core/Core.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Orange
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return sg_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return sg_ClientLogger; }
	private:
		static Ref<spdlog::logger> sg_CoreLogger;
		static Ref<spdlog::logger> sg_ClientLogger;
	};
}

// 内核日志宏函数
#define OG_CORE_TRACE(...)          ::Orange::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OG_CORE_INFO(...)           ::Orange::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OG_CORE_WARN(...)           ::Orange::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OG_CORE_ERROR(...)          ::Orange::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OG_CORE_CRITICAL(...)       ::Orange::Log::GetCoreLogger()->critical(__VA_ARGS__)

// 客户端日志宏函数
#define OG_CLIENT_TRACE(...)        ::Orange::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OG_CLIENT_INFO(...)         ::Orange::Log::GetClientLogger()->info(__VA_ARGS__)
#define OG_CLIENT_WARN(...)         ::Orange::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OG_CLIENT_ERROR(...)        ::Orange::Log::GetClientLogger()->error(__VA_ARGS__)
#define OG_CLIENT_CRITICAL(...)     ::Orange::Log::GetClientLogger()->critical(__VA_ARGS__)