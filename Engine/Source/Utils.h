#pragma once

#include <concepts>

namespace Engine
{
    template <class T>
    concept NumericType = std::is_arithmetic_v<T>;

    template <NumericType Number>
    struct Point2D
    {
        Number x;
        Number y;
    };

    template <NumericType Number>
    struct Point3D
    {
        Number x;
        Number y;
        Number z;
    };
}