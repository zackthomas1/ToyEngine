#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

namespace ToyEngine
{
  class Log {
  public:
    static void Init(); 

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

  private: 
    static std::shared_ptr<spdlog::logger> s_CoreLogger; 
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };
}

#ifdef TY_DEBUG
  // Core log macros
  #define TY_CORE_TRACE(...)    :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)
  #define TY_CORE_INFO(...)   :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)
  #define TY_CORE_WARN(...)   :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
  #define TY_CORE_ERROR(...)  :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
  #define TY_CORE_CRITICAL(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)

  // client log macros
  #define TY_TRACE(...) :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)
  #define TY_INFO(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)
  #define TY_WARN(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
  #define TY_ERROR(...) :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)
  #define TY_CRITICAL(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#else
  #define TY_CORE_TRACE(...)
  #define TY_CORE_INFO(...)
  #define TY_CORE_WARN(...)
  #define TY_CORE_ERROR(...)
  #define TY_CORE_FATAL(...)

  // client log macros
  #define TY_TRACE(...)
  #define TY_INFO(...)
  #define TY_WARN(...)
  #define TY_ERROR(...)
  #define TY_CORE_FATAL(...)

#endif // TY_DEBUG