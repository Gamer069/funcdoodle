#pragma once

#include "Gui.h"
#include "imgui.h"
#include "nfd.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#define TOML_EXCEPTIONS 0

#include <toml++.h>

#include "MacroUtils.h"

#include "UUID.h"

namespace FuncDoodle {
	namespace Themes {
		struct CustomTheme {
			const char* Name;
			const char* Author;
			ImGuiStyle Style;
			UUID Uuid;
			CustomTheme() : Uuid(UUID()), Name(""), Author(""), Style(ImGuiStyle()) {}
			CustomTheme(const char* name, const char* author, ImGuiStyle style, UUID uuid) : Uuid(uuid), Name(name), Author(author), Style(style) {}
		};

		inline std::map<UUID, CustomTheme> g_Themes;
		inline bool g_ThemeEditorOpen = false;
		inline bool g_SaveThemeOpen = false;
		inline void ThemeEditor() {
			if (g_ThemeEditorOpen) {
				if (ImGui::Begin("Theme editor", &g_ThemeEditorOpen,
								 ImGuiWindowFlags_NoCollapse)) {
					ImGuiStyle& style = ImGui::GetStyle();

					ImVec2 windowSize = ImGui::GetWindowSize();
					float windowWidth = windowSize.x;

					// Calculate the number of col based on the window width
					int numCol = static_cast<int>(
						windowWidth /
						200);  // Each column has a minimum width of 100px
					numCol = ImMax(1, numCol);

					ImGui::Columns(numCol, nullptr, false);
					for (unsigned char col = 0; col < ImGuiCol_COUNT; ++col) {
						ImGui::PushID(col);
						ImVec4& color = style.Colors[col];

						if (ImGui::ColorEdit4(ImGui::GetStyleColorName(col),
											  (float*)&color)) {
							style.Colors[col] = color;
						}

						if ((col + 1) % numCol == 0) {
							ImGui::NextColumn();
						}

						ImGui::PopID();
					}

					// End the cols
					ImGui::Columns(1);

					if (ImGui::Button("Close") ||
						ImGui::IsKeyPressed(ImGuiKey_Escape) ||
						ImGui::IsKeyPressed(ImGuiKey_Enter) ||
						ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
						g_ThemeEditorOpen = false;
					}

					ImGui::End();
				}
			}
		}
		static CustomTheme* g_LastLoadedTheme;
		inline CustomTheme* LoadThemeFromFile(const char* path) {
			using namespace std::string_view_literals;

			toml::parse_result result =
				toml::parse_file(std::string_view(path));
			if (!result) {
				FUNC_WARN("Failed to parse theme file... it's probably not "
						  "valid TOML");
				FUNC_WARN("error: " << result.error());
				return nullptr;
			}

			toml::table table = result.table();
			if (!table.contains("meta"sv) || !table.contains("colors"sv)) {
				FUNC_WARN(
					"Theme file is missing required fields (meta/colors)");
				return nullptr;
			}

			toml::table* meta = table.get("meta"sv)->as_table();
			const char* name = meta->get("name")->as_string()->get().c_str();
			const char* author =
				meta->get("author")->as_string()->get().c_str();
			const char* uuid = meta->get("uuid")->as_string()->get().c_str();

			// Allocate memory for name and author (because TOML parser returns
			// temporary strings)
			char* name_copy = (char*)malloc(strlen(name) + 1);
			char* author_copy = (char*)malloc(strlen(author) + 1);
			char* uuid_copy = (char*)malloc(strlen(uuid) + 1);

			if (!name_copy || !author_copy || !uuid_copy) {
				if (name_copy)
					free(name_copy);
				if (author_copy)
					free(author_copy);
				if (uuid_copy)
					free(uuid_copy);
				FUNC_ERR("Memory allocation failed!");
				return nullptr;
			}

			strcpy(name_copy, name);
			strcpy(author_copy, author);
			strcpy(uuid_copy, uuid);

			toml::table* colors = table.get("colors"sv)->as_table();
			ImGuiStyle style = ImGui::GetStyle();

			for (const auto& [k, v] : *colors) {
				if (!v.is_array())
					continue;

				// Convert key to an integer safely
				int parsed;
				try {
					parsed = std::stoi(std::string(k.str()));
				} catch (...) {
					FUNC_WARN("Invalid color key: " << k.str());
					continue;
				}

				if (parsed < 0 || parsed >= ImGuiCol_COUNT) {
					FUNC_WARN("Invalid ImGui color index: " << parsed);
					continue;
				}

				toml::array arr = *v.as_array();
				if (arr.size() != 4)
					continue;

				style.Colors[parsed] =
					ImVec4(arr.get(0)->as_floating_point()->get(),
						   arr.get(1)->as_floating_point()->get(),
						   arr.get(2)->as_floating_point()->get(),
						   arr.get(3)->as_floating_point()->get());
			}
			style.WindowRounding = 1;
			style.Alpha = 1;

			g_LastLoadedTheme = new CustomTheme{name_copy, author_copy, style,
												UUID::FromString(uuid_copy)};

			return g_LastLoadedTheme;
		}
		inline void LoadThemes(std::filesystem::path path) {
			if (std::filesystem::exists(path) &&
				std::filesystem::is_directory(path)) {
				for (std::filesystem::directory_entry e :
					 std::filesystem::directory_iterator(path)) {
					if (!e.is_regular_file()) {
						FUNC_GRAY("Skipping "
								  << e.path()
								  << "because it isn't a regular file");
						continue;
					}
					std::string pathStr = e.path().string();
					const char* path = pathStr.c_str();
					CustomTheme* theme = LoadThemeFromFile(path);
					if (theme) {
						g_Themes.emplace(theme->Uuid, *theme);
					}
				}
			} else {
				FUNC_FATAL("Failed to load themes -- either the themes/ "
						   "directory doesn't exist, or it isn't a directory");
			}
			// std::sort(g_Themes.begin(), g_Themes.end(), [](const CustomTheme& a, const CustomTheme& b) { return strcasecmp(a.Name, b.Name) < 0; });
			std::vector<std::pair<UUID, CustomTheme>> themeVec(g_Themes.begin(), g_Themes.end());

			// Selection sort (or any other sorting algorithm)
			size_t n = themeVec.size();
			for (size_t i = 0; i < n - 1; i++) {
				size_t minIndex = i;
				for (size_t j = i + 1; j < n; j++) {
					if (strcasecmp(themeVec[j].second.Name, themeVec[minIndex].second.Name) < 0) {
						minIndex = j;
					}
				}
				if (minIndex != i) {
					std::swap(themeVec[i], themeVec[minIndex]);
				}
			}

			// Clear and refill the map in sorted order
			g_Themes.clear();
			for (auto& [uuid, theme] : themeVec) {
				g_Themes.emplace(uuid, theme);
			}
		}
		inline void SaveCurrentTheme() {
			if (g_SaveThemeOpen) {
				ImGui::Begin("Save current theme", &g_SaveThemeOpen);
				static char themeName[100] = "";
				ImGui::InputText("Name", themeName, 100);
				static char themeAuthor[50] = "";
				ImGui::InputText("Author", themeAuthor, 50);
				if (ImGui::Button("Save")) {
					nfdchar_t* savePath = 0;
					nfdresult_t res = NFD_SaveDialog("toml", 0, &savePath);
					if (res == NFD_OKAY) {
						using namespace std::string_view_literals;
						toml::table theme = toml::table();
						toml::table meta = toml::table();
						meta.insert("name"sv, themeName);
						meta.insert("author"sv, themeAuthor);
						meta.insert("uuid"sv, UUID::Gen().ToString());
						theme.insert("meta"sv, meta);
						toml::table colors = toml::table();
						ImGuiStyle& style = ImGui::GetStyle();
						for (unsigned char i = 0; i < ImGuiCol_COUNT; ++i) {
							std::string is = std::to_string(i);
							std::string_view iv = std::string_view(is);
							colors.insert(iv, toml::array{style.Colors[i].x,
									style.Colors[i].y,
									style.Colors[i].z,
									style.Colors[i].w});
						}
						theme.insert("colors"sv, colors);

						std::ofstream f(savePath);
						if (!f) {
							FUNC_ERR("Failed to open file...");
							return;
						}
						FUNC_INF("saving current theme to " << savePath
								<< "...");
						f << theme;
						f.close();
						g_SaveThemeOpen = false;
					} else if (res == NFD_ERROR) {
						FUNC_ERR("Failed to open save theme dialog: "
								<< NFD_GetError());
					}
				}
				ImGui::End();
			}
		}
	}  // namespace Themes
}  // namespace FuncDoodle
