#include "pch.h"
#include "log.h"

namespace ToyEngine
{
  std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

  void Log::Init()
  {
    // Pattern - '[{logger}] [{Time}] [{File name}]:{line of logging statement}] {logging statement}'
    spdlog::set_pattern("%^[%T] [%n] [%s:%#] %v%$");

    s_CoreLogger = spdlog::stdout_color_mt("ToyEngine");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("App");
    s_ClientLogger->set_level(spdlog::level::trace);
  }
}