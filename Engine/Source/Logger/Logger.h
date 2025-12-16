#pragma once
#include <string_view>
namespace Engine
{
class Logger
{
public:
    static void LogInfo(const std::string_view module, const std::string_view message);
    static void LogWarning(const std::string_view module, const std::string_view message);
    static void LogError(const std::string_view module, const std::string_view message);
};

}; // namespace Engine