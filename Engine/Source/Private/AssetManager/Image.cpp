#include "AssetManager/Image.h"

#include "Core/Util/Utils.h"
#include "AssetManager/AssetManager.h"

#include "Error/Error.h"
#include "Core/CustomAssert.h"
#include "Logger/Logger.h"   

#include "stb_image.h"

DECLARE_LOG_CATEGORY(Image);

namespace Engine
{
CImage::CImage(const FPath& RelImagePath, bool ShouldLoadImmediately)
{
    if (RelImagePath.empty())
    {
        LOG(Image, ERROR, "CImage loading failed: empty path");
        ImagePath = "";
    }
    else
    {
        ImagePath = RelImagePath;
    }

    if (ShouldLoadImmediately && !RelImagePath.empty())
    {
        SGenericError Err = Load();
        if (Err)
        {
            LOG(Image, ERROR, "Image loading failed: {}", Err);
        }
    }
}

CImage::~CImage()
{
    Free();
}

SGenericError CImage::Load()
{
    SGenericError Err;

    if (Data)
    {
        LOG(Image, WARN, "Overriding Existing Image!");
        Free();
    }

    FPath FullPath = CAssetManager::FindFile(ImagePath);
    if (FullPath.empty())
    {
        return { -1, "File Not Found: " + ImagePath.string() };
    }

    return LoadImage_stbi(FullPath);
}

void CImage::Free()
{
    FreeImage_stbi();
}

SGenericError CImage::LoadImage_stbi(const FPath& ImagePathFull)
{
    stbi_set_flip_vertically_on_load(1);
    ASSERT(!ImagePathFull.empty(), "Empty File path for image to load");

    int width, height, channels, desired_channels = 4;
    Data = stbi_load(ImagePathFull.string().c_str(), &width, &height, &channels, desired_channels);
    if (!Data)
    {
        return { -1, "STBI Image Load Failed: {} for file {}", stbi_failure_reason(), ImagePath.string() };
    }

    Dimensions.x = (uint32_t)width;
    Dimensions.y = (uint32_t)height;

    return {};
}

void CImage::FreeImage_stbi()
{
    if (Data) stbi_image_free(Data);
}

} // namespace Engine
