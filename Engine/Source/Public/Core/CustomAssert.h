#pragma once
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(ASSERT);

#define ASSERT(x, fmt, ...)                                                \
    do                                                                     \
    {                                                                      \
        if (!(x))                                                          \
        {                                                                  \
            LOG(ASSERT, ERROR,                                             \
                "[{}-{}] ASSERTION failed: {}", __FILE__, __LINE__, fmt,   \
                #x __VA_OPT__(,) __VA_ARGS__);                             \
        }                                                                  \
    } while(0)
