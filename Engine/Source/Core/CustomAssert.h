#pragma once
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(ASSERT);

#if WINDOWS
#define ASSERT(x) if (!(x)) LOG(ASSERT, ERROR, "ASSERTION {} failed!", x)
#elif LINUX
#define ASSERT(x) if (!(x)) LOG(ASSERT, ERROR, "[{}-{}] ASSERTION {} failed!", __FILE__, __LINE__, x)
#endif
