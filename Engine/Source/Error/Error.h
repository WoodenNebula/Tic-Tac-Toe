#pragma once

#include <ostream>
#include <sstream>
#include <string_view>
#include <format>

namespace Engine
{
struct SGenericError
{
    int ErrorCode{ 0 };
    std::string_view ErrorMessage{ "NONE" };

    SGenericError() = default;
    SGenericError(int inErrorCode, const std::string_view& inErrorMessage) : ErrorCode(inErrorCode), ErrorMessage(inErrorMessage) {}

    inline operator bool() const
    {
        return ErrorCode != 0;
    }

    inline operator std::string() const
    {
        std::stringstream ss;
        ss << "{ " << ErrorCode << " : " << ErrorMessage << " }";

        return ss.str();
    }

    inline friend std::ostream& operator<<(std::ostream& os, const SGenericError& error)
    {
        os << error;
        return os;
    }
};

}
template<>
struct std::formatter<Engine::SGenericError> : std::formatter<std::string>
{
    auto format(const Engine::SGenericError& error, auto& ctx) const
    {
        return std::formatter<std::string>::format(std::format("{{ {} : {} }}", error.ErrorCode, error.ErrorMessage), ctx);
    }
};