#pragma once
#include <iostream>
#include <format>
#include <string_view>

namespace Engine
{
enum ELogLevel
{
    Trace = 0,
    Info,
    Warn,
    Error,
    Fatal
};

#define TRACE Engine::ELogLevel::Trace
#define INFO Engine::ELogLevel::Info
#define WARN Engine::ELogLevel::Warn
#define ERROR Engine::ELogLevel::Error
#define FATAL Engine::ELogLevel::Fatal

#ifdef DEBUG
#define ENGINE_LOG_LEVEL TRACE
#else
#define ENGINE_LOG_LEVEL INFO
#endif

struct LogCategory
{
    std::string_view Name;
    ELogLevel LogLevel;

    LogCategory(const std::string_view name, ELogLevel level = ENGINE_LOG_LEVEL)
        : Name(name), LogLevel(level)
    {
    }
};

inline static void LogWrite(const LogCategory& category, ELogLevel level, std::string_view msg)
{
    if (level < category.LogLevel)
        return;

    std::string logLevelStr;

    switch (category.LogLevel)
    {
    case ELogLevel::Trace: logLevelStr = "TRACE"; break;
    case ELogLevel::Info: logLevelStr = "INFO"; break;
    case ELogLevel::Warn: logLevelStr = "WARNING"; break;
    case ELogLevel::Error: logLevelStr = "ERROR"; break;
    case ELogLevel::Fatal: logLevelStr = "FATAL"; break;
    default: break;
    }

    if (level == ELogLevel::Fatal)
    {
        std::cerr
            << "[" << logLevelStr << "] - "
            << "[" << category.Name << "] "
            << msg << '\n';
    }
    else
    {
        std::cout
            << "[" << logLevelStr << "] - "
            << "[" << category.Name << "] "
            << msg << '\n';
    }
}

#define DECLARE_LOG_CATEGORY(CategoryName) \
inline Engine::LogCategory Log##CategoryName{#CategoryName};

#define DECLARE_LOG_CATEGORY_LEVEL(CategoryName, Level) \
inline Engine::LogCategory Log##CategoryName{#CategoryName, Level };

#define LOG(Category, Level, Fmt, ...) \
    do { \
        Engine::LogWrite( \
            Log##Category, \
            Level, \
            std::format(Fmt __VA_OPT__(,) __VA_ARGS__) \
        ); \
    } while (0)
}; // namespace Engine