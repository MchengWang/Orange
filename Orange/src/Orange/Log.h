#pragma once

#include "Core.h"

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Orange
{
	class ORANGE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sg_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sg_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> sg_CoreLogger;
		static std::shared_ptr<spdlog::logger> sg_ClientLogger;
	};
}

// 内核日志宏函数
#define OG_CORE_TRACE(...)       ::Orange::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OG_CORE_INFO(...)        ::Orange::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OG_CORE_WARN(...)        ::Orange::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OG_CORE_ERROR(...)       ::Orange::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OG_CORE_FATAL(...)       ::Orange::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// 客户端日志宏函数
#define OG_CLIENT_TRACE(...)     ::Orange::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OG_CLIENT_INFO(...)      ::Orange::Log::GetClientLogger()->info(__VA_ARGS__)
#define OG_CLIENT_WARN(...)      ::Orange::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OG_CLIENT_ERROR(...)     ::Orange::Log::GetClientLogger()->error(__VA_ARGS__)
#define OG_CLIENT_FATAL(...)     ::Orange::Log::GetClientLogger()->fatal(__VA_ARGS__)