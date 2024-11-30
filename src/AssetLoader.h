#pragma once

#include <filesystem>
#include "Gui.h"

namespace FuncDoodle {
    class AssetLoader {
        public:
            AssetLoader(const std::filesystem::path& assetsPath);
            ~AssetLoader();
            inline const std::filesystem::path& GetPath() const { return m_AssetsPath; };
            inline void SetPath(const std::filesystem::path& assetsPath) { m_AssetsPath = assetsPath; };
            void LoadAssets();
            void UnloadAssets();
            void RenderImage(const char* name, ImDrawList* drawList, const ImVec2& pos, const ImVec2& size, const ImVec4& tint = ImVec4(1, 1, 1, 1));
            uint32_t LoadImage(const char* name);
        private:
            std::filesystem::path m_AssetsPath;
    };
}