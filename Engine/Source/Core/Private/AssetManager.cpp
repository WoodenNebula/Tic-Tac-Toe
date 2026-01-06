#include "Core/AssetManager.h"
#include "Logger/Logger.h"
#include <filesystem>
#include <string_view>
#include <vector>
namespace Engine{
#ifdef PROJECT_ROOT
inline static constexpr std::string_view s_ProjectRoot(PROJECT_ROOT);
#elif 
inline static constexpr FPath s_ProjectRoot = std::filesystem::current_path();
#endif
inline static std::vector<FPath> s_AssetPathRoots { "Engine", "Tic-Tac-Toe"};


void CAssetManager::RegisterAssetRoot(const FPath& ProjectAssetRoot) { s_AssetPathRoots.emplace_back(ProjectAssetRoot); }

FPath CAssetManager::FindFile(const FPath& RelPath)
{
    for (const auto& assetPath : s_AssetPathRoots)
    {
        FPath fullPath = s_ProjectRoot / assetPath / RelPath;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath;
        }
    }
    return "";
}
}
