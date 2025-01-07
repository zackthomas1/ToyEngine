#include "pch.h"
#include "log.h"

namespace ToyEngine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// Set the general pattern of logging statements 
		// Pattern - '[{File name}]: {line of logging statement}] [{Time}] {logger}: {logging statement}'
		spdlog::set_pattern("[%s:%#] \t %^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("ToyEngine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}

