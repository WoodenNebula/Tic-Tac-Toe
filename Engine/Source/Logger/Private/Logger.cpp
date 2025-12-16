#include "Logger/Logger.h"

#include <iostream>
namespace Engine
{

void Logger::LogInfo(const std::string_view module, const std::string_view message)
{
    std::cout << "[INFO]: " << message << std::endl;
}
void Logger::LogWarning(const std::string_view module, const std::string_view message)
{
    std::cout << "[WARNING]: " << message << std::endl;
}
void Logger::LogError(const std::string_view module, const std::string_view message)
{
    std::cerr << "[ERROR]: " << message << std::endl;
}

}; // namespace Engine