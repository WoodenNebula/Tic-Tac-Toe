#define STB_IMAGE_IMPLEMENTATION
#include "Core/CustomAssert.h"
#define STBI_ASSERT(x) ASSERT(x, "STBI_ASSERT: ")
#include "stb_image.h"