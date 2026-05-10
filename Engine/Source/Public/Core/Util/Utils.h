#pragma once

#include <concepts>
#include <string>

namespace Engine
{
template <class T>
concept NumericType = std::is_arithmetic_v<T>;

template <NumericType Number>
struct Point2D
{
    Number x;
    Number y;

    std::string ToString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

template <NumericType Number>
struct Point3D
{
    Number x;
    Number y;
    Number z;

    std::string ToString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
};

}; // namespace Engine