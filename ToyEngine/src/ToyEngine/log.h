#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

namespace ToyEngine
{
  class Log {
  public:
    /// @brief Initializes the logging system for the engine and client application.
    /// This function is called from the application entry point.
    static void Init();

    /// @brief Returns a reference to the core logger used by the engine.
    /// @return A shared pointer to the core logger.
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    /// @brief Returns a reference to the client logger used by the application.
    /// @return A shared pointer to the client logger.
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

  private:
    /// @brief The core logger used by the engine.
    static std::shared_ptr<spdlog::logger> s_CoreLogger;

    /// @brief The client logger used by the application.
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };
}

#ifdef TY_DEBUG
//  Core log macros
/* Macro to log a core engine system message with trace level. */
#define TY_CORE_TRACE(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)

/* Macro to log a core engine system message with info level. */
#define TY_CORE_INFO(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)

/* Macro to log a core engine system message with warn level. */
#define TY_CORE_WARN(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)

/* Macro to log a core engine system message with error level. */
#define TY_CORE_ERROR(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)

/* Macro to log a core engine system message with critical level. */
#define TY_CORE_CRITICAL(...) :: ToyEngine::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)

// Client log macros
/* Macro to log a client  message with trace level. */
#define TY_TRACE(...) :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)

/* Macro to log a client  message with info level. */
#define TY_INFO(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)

/* Macro to log a client  message with warn level. */
#define TY_WARN(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
  
/* Macro to log a client  message with error level. */
#define TY_ERROR(...) :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, __VA_ARGS__)

/* Macro to log a client  message with cirtical level. */
#define TY_CRITICAL(...)  :: ToyEngine::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)
#else
//  Core log macros
#define TY_CORE_TRACE(...)
#define TY_CORE_INFO(...)
#define TY_CORE_WARN(...)
#define TY_CORE_ERROR(...)
#define TY_CORE_FATAL(...)

// Client log macros
#define TY_TRACE(...)
#define TY_INFO(...)
#define TY_WARN(...)
#define TY_ERROR(...)
#define TY_CORE_FATAL(...)

#endif // TY_DEBUG