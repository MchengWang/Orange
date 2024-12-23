#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Orange
{
	std::shared_ptr<spdlog::logger> Log::sg_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::sg_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		sg_CoreLogger = spdlog::stdout_color_mt("ORANGE");
		sg_CoreLogger->set_level(spdlog::level::trace);

		sg_ClientLogger = spdlog::stdout_color_mt("APP");
		sg_ClientLogger->set_level(spdlog::level::trace);
	}
}