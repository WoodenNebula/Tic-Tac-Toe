#pragma once

namespace Engine
{

template <typename T>
class Singleton
{
public:
    static T& Get()
    {
        static T Instance;
        return Instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;
};


}; // namespace Engine