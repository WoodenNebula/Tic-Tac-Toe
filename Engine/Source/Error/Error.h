#pragma once

#include <ostream>
#include <string_view>

namespace Engine
{
    struct SGenericError
    {
        int ErrorCode{ 0 };
        std::string_view ErrorMessage{ "NONE" };

        SGenericError() = default;
        SGenericError(int inErrorCode, const std::string_view &inErrorMessage) : ErrorCode(inErrorCode), ErrorMessage(inErrorMessage) {}

        inline operator bool() const
        {
            return ErrorCode != 0;
        }

        inline friend std::ostream &operator<<(std::ostream &os, const SGenericError &error)
        {
            os << "[ERROR] : { " << error.ErrorCode << " : " << error.ErrorMessage << " }\n";
            return os;
        }


    };
}