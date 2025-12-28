#pragma once
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(ASSERT);

#define ASSERT(x) if (!(x)) LOG(ASSERT, ERROR, "ASSERTION {} failed!", x)