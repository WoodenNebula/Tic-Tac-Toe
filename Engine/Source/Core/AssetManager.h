#pragma once

#include <filesystem>

namespace Engine {
using FPath = std::filesystem::path;
class CAssetManager
{
    public:
        /// @param RelPath: Path to the file asset to load, relative to the Project Dir
        static FPath FindFile(const FPath& RelPath);
        static void RegisterAssetRoot(const FPath& ProjectAssetRoot);
};
}
