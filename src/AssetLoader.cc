#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Gui.h"

#include <iostream>
#include <filesystem>

#include <glad/glad.h>

namespace FuncDoodle {
    AssetLoader::AssetLoader(const std::filesystem::path& assetsPath) : m_AssetsPath(assetsPath) {
        LoadAssets();
    }
    AssetLoader::~AssetLoader() {
        UnloadAssets();
    }

    void AssetLoader::LoadAssets() {
        std::cout << "Finding assets path..." << std::endl;
        if (std::filesystem::exists(m_AssetsPath)) {
            std::cout << "Successfully found assets path: " << m_AssetsPath << std::endl;
        } else {
            std::cerr << "Assets path does not exist: " << m_AssetsPath << std::endl;
            std::exit(-1);
        }
    }
    void AssetLoader::UnloadAssets() {
        std::cout << "Unloading assets..." << std::endl;
        std::cout << "Assets unloaded." << std::endl;
    }
    
    // NOTE: this function doesnt support svg because stb_image doesn't support it.
    void AssetLoader::RenderImage(const char* name, ImDrawList* drawList, const ImVec2& pos, const ImVec2& size, const ImVec4& tint) {
        int width, height, channels;
        unsigned char* data = stbi_load((m_AssetsPath / name).c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load image " << (m_AssetsPath / name).c_str() << std::endl;
            std::cerr << "Error: " << stbi_failure_reason() << std::endl;
            std::cerr << "Image dimensions: " << width << "x" << height << ", channels: " << channels << std::endl;
            std::cerr << "Attempted to load from assets path: " << m_AssetsPath << std::endl;
            std::exit(-1);
        }

        uint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        drawList->AddImage((void*)(intptr_t)textureID, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(tint));
    }
    uint AssetLoader::LoadImage(const char* name) {
        int width, height, channels;
        unsigned char* data = stbi_load((m_AssetsPath / name).c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load image " << (m_AssetsPath / name).c_str() << std::endl;
            std::cerr << "Error: " << stbi_failure_reason() << std::endl;
            std::cerr << "Image dimensions: " << width << "x" << height << ", channels: " << channels << std::endl;
            std::cerr << "Attempted to load from assets path: " << m_AssetsPath << std::endl;
            std::exit(-1);
        }

        uint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return textureID;
    }
}
