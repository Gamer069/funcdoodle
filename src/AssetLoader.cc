#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Gui.h"

#include <filesystem>
#include <iostream>

#include "MacroUtils.h"

#include <glad/glad.h>

namespace FuncDoodle {
	AssetLoader::AssetLoader(const std::filesystem::path& assetsPath)
		: m_AssetsPath(assetsPath) {
		LoadAssets();
	}
	AssetLoader::~AssetLoader() {
		UnloadAssets();
	}

	void AssetLoader::LoadAssets() {
		FUNC_DBG("Finding assets path...");
		if (std::filesystem::exists(m_AssetsPath)) {
			FUNC_DBG("Found assets path @" + m_AssetsPath.string());
		} else {
			FUNC_WARN("Failed to find assets path -- " + m_AssetsPath.string() + " doesn't exist");
			std::exit(-1);
		}
	}
	void AssetLoader::UnloadAssets() {
		FUNC_DBG("Unloading assets even though there aren't even any assets to unload, this is a useless function (for now)-");
		FUNC_DBG("\"Unloaded assets\"...");
	}

	// NOTE: this function doesnt support svg because stb_image doesn't support
	// it.
	void AssetLoader::RenderImage(const char* name, ImDrawList* drawList,
								  const ImVec2& pos, const ImVec2& size,
								  const ImVec4& tint) {
		int width, height, channels;
		unsigned char* data = stbi_load((m_AssetsPath / name).string().c_str(),
										&width, &height, &channels, 0);
		if (!data) {
			FUNC_WARN("Failed to load image " + (m_AssetsPath / name).string());
			FUNC_WARN("Error: " << stbi_failure_reason());
			FUNC_WARN("Image dim: " << width << "x" << height << ", channels: " << channels);
			FUNC_WARN("Tried to load from assets path -- " + m_AssetsPath.string());
			std::exit(-1);
		}

		uint32_t textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					 GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		drawList->AddImage((ImTextureID)(intptr_t)textureID, pos,
						   ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0),
						   ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(tint));
	}
	uint32_t AssetLoader::LoadImage(const char* name) {
		int width, height, channels;
		unsigned char* data = stbi_load((m_AssetsPath / name).string().c_str(),
										&width, &height, &channels, 0);
		if (!data) {
			FUNC_WARN("Failed to load image " + (m_AssetsPath / name).string());
			FUNC_WARN("Error: " << stbi_failure_reason());
			FUNC_WARN("Image dim: " << width << "x" << height << ", channels: " << channels);
			FUNC_WARN("Tried to load from assets path -- " + m_AssetsPath.string());
			std::exit(-1);
		}

		uint32_t textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					 GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		return textureID;
	}
}  // namespace FuncDoodle
