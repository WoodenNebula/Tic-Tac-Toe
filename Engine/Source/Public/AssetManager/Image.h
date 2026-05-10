#pragma once

#include "Core/Util/Utils.h"
#include "AssetManager/AssetManager.h"

#include "Error/Error.h"
#include "Core/CustomAssert.h"
#include "Logger/Logger.h"   

#include "stb_image.h"

namespace Engine
{
class CImage
{
public:
    CImage() = default;
    CImage(const FPath& RelImagePath, bool ShouldLoadImmediately = false);
    ~CImage();

    inline bool IsLoaded() const { return Data != nullptr; }
    inline bool HasValidPath() const { return !ImagePath.empty(); }

    SGenericError Load();
    void Free();

protected:
    SGenericError LoadImage_stbi(const FPath& ImagePathFull);
    void FreeImage_stbi();

protected:
    /// <summary>
    ///  Relative path to image file as passed during initialization
    /// </summary>
    FPath ImagePath;


public:
    Point2D<uint32_t> Dimensions{ 0,0 };
    unsigned char* Data{ nullptr };
};

} // namespace Engine
