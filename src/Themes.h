#pragma once

#include "Gui.h"
#include "imgui.h"
#include "nfd.h"

#include <cstdlib>
#include <string>
#include <string_view>

#define TOML_EXCEPTIONS 0

#include <toml++.h>

#include "MacroUtils.h"

namespace FuncDoodle {
	namespace Themes {
		struct CustomTheme {
			const char* Name;
			const char* Author;
			ImGuiStyle* Style;
		};

		inline std::vector<CustomTheme> g_Themes = std::vector{
			CustomTheme {
				"FuncDoodle style",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Dark",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Light",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Classic",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Catppuccin Mocha",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Catppuccin Macchiato",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Catppuccin Frappe",
				"(built-in)",
				nullptr,
			},
			CustomTheme {
				"Catppuccin Latte",
				"(built-in)",
				nullptr,
			},
		};
		inline bool g_ThemeEditorOpen = false;
		inline bool g_SaveThemeOpen = false;
		inline ImGuiStyle FuncDoodleStyle() {
			ImGuiStyle style = ImGui::GetStyle();
			ImGuiIO& io = ImGui::GetIO();
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
			style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
			style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
			style.WindowRounding = 10.0f;
			style.FrameRounding = 5.0f;
			style.PopupRounding = 12.0f;
			style.ScrollbarRounding = 10.0f;
			style.GrabRounding = 6.0f;
			style.TabRounding = 12.0f;
			style.ChildRounding = 12.0f;
			style.WindowPadding = ImVec2(10, 10);
			style.FramePadding = ImVec2(8, 8);
			style.ItemSpacing = ImVec2(10, 10);
			style.IndentSpacing = 20.0f;
			style.ScrollbarSize = 16.0f;
			ImVec4* colors = style.Colors;
			colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			colors[ImGuiCol_Border] = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			colors[ImGuiCol_CheckMark] =
				ImVec4(0.0f, 1.0f, 0.0f, 1.0f);	 // Bright green
			colors[ImGuiCol_SliderGrab] =
				ImVec4(0.1f, 0.5f, 0.8f, 1.0f);	 // Bright blue
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1f, 0.6f, 0.9f, 1.0f);
			colors[ImGuiCol_Button] = ImVec4(0.1f, 0.5f, 0.8f, 1.0f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.6f, 0.9f, 1.0f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
			colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
			colors[ImGuiCol_Separator] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
			colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
			colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
			colors[ImGuiCol_DockingPreview] = ImVec4(0.2f, 0.3f, 0.5f, 0.7f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			colors[ImGuiCol_ChildBg] =
				ImVec4(0.2f, 0.2f, 0.2f, 0.0f);	 // Make child background transparent
			colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);

			// Optional: You can adjust global scaling or other parameters here if
			// needed
			style.Alpha = 1.0f;  // Fully opaque
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				style.WindowRounding = 1.0f;
			}
			return style;
		}

		inline ImGuiStyle CatppuccinLatteStyle() {
			// ImGui::StyleColorsDark();
			ImGuiStyle style = ImGui::GetStyle();
			auto rgb = [](unsigned char r, unsigned char g, unsigned char b) {
				return ImGui::ColorConvertU32ToFloat4(IM_COL32(r, g, b, 255));
			};
			ImVec4 rosewater = rgb(220, 138, 120);
			ImVec4 flamingo = rgb(221, 120, 120);
			ImVec4 pink = rgb(234, 118, 203);
			ImVec4 mauve = rgb(136, 57, 239);
			ImVec4 red = rgb(210, 15, 57);
			ImVec4 maroon = rgb(230, 69, 83);
			ImVec4 peach = rgb(254, 100, 11);
			ImVec4 yellow = rgb(223, 142, 29);
			ImVec4 green = rgb(64, 160, 43);
			ImVec4 teal = rgb(23, 146, 153);
			ImVec4 sky = rgb(4, 165, 229);
			ImVec4 sapphire = rgb(32, 159, 181);
			ImVec4 blue = rgb(30, 102, 245);
			ImVec4 lavender = rgb(114, 135, 253);
			ImVec4 text = rgb(76, 79, 105);
			ImVec4 subtext1 = rgb(92, 95, 119);
			ImVec4 subtext0 = rgb(108, 111, 133);
			ImVec4 overlay2 = rgb(124, 127, 147);
			ImVec4 overlay1 = rgb(140, 143, 161);
			ImVec4 overlay0 = rgb(156, 160, 176);
			ImVec4 surface2 = rgb(172, 176, 190);
			ImVec4 surface1 = rgb(188, 192, 204);
			ImVec4 surface0 = rgb(204, 208, 218);
			ImVec4 base_latte = rgb(239, 241, 245);
			ImVec4 mantle_latte = rgb(230, 233, 239);
			ImVec4 crust_latte = rgb(220, 224, 232);
			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_latte;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_latte;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_latte;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_latte;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_latte;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_latte;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_latte;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_latte;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_latte;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_latte;  // Unfocused active tab
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_latte;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_latte;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_latte;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_latte;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_latte;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_latte;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_latte;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_latte;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_latte;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_latte;  // Unfocused active tab
			return style;
		}

		inline ImGuiStyle CatppuccinFrappeStyle() {
			ImGuiStyle style = ImGui::GetStyle();
			auto rgb = [](unsigned char r, unsigned char g, unsigned char b) {
				return ImGui::ColorConvertU32ToFloat4(IM_COL32(r, g, b, 255));
			};
			ImVec4 rosewater = rgb(242, 213, 207);
			ImVec4 flamingo = rgb(238, 190, 190);
			ImVec4 pink = rgb(244, 184, 228);
			ImVec4 mauve = rgb(202, 158, 230);
			ImVec4 red = rgb(231, 130, 132);
			ImVec4 maroon = rgb(234, 153, 156);
			ImVec4 peach = rgb(239, 159, 118);
			ImVec4 yellow = rgb(229, 200, 144);
			ImVec4 green = rgb(166, 209, 137);
			ImVec4 teal = rgb(129, 200, 190);
			ImVec4 sky = rgb(153, 209, 219);
			ImVec4 sapphire = rgb(133, 193, 220);
			ImVec4 blue = rgb(140, 170, 238);
			ImVec4 lavender = rgb(186, 187, 241);
			ImVec4 text = rgb(198, 208, 245);
			ImVec4 subtext1 = rgb(181, 191, 226);
			ImVec4 subtext0 = rgb(165, 173, 206);
			ImVec4 overlay2 = rgb(148, 156, 187);
			ImVec4 overlay1 = rgb(131, 139, 167);
			ImVec4 overlay0 = rgb(115, 121, 148);
			ImVec4 surface2 = rgb(98, 104, 128);
			ImVec4 surface1 = rgb(81, 87, 109);
			ImVec4 surface0 = rgb(65, 69, 89);
			ImVec4 base_frappe = rgb(48, 52, 70);
			ImVec4 mantle_frappe = rgb(41, 44, 60);
			ImVec4 crust_frappe = rgb(35, 38, 52);
			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_frappe;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_frappe;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_frappe;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_frappe;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_frappe;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_frappe;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_frappe;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_frappe;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_frappe;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_frappe;  // Unfocused active tab
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_frappe;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_frappe;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_frappe;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_frappe;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_frappe;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_frappe;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_frappe;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_frappe;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_frappe;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_frappe;  // Unfocused active tab
			return style;
		}

		inline ImGuiStyle CatppuccinMacchiatoStyle() {
			ImGuiStyle style = ImGui::GetStyle();
			auto rgb = [](unsigned char r, unsigned char g, unsigned char b) {
				return ImGui::ColorConvertU32ToFloat4(IM_COL32(r, g, b, 255));
			};
			ImVec4 rosewater = rgb(244, 219, 214);
			ImVec4 flamingo = rgb(249, 198, 198);
			ImVec4 pink = rgb(245, 189, 230);
			ImVec4 mauve = rgb(198, 160, 246);
			ImVec4 red = rgb(237, 135, 150);
			ImVec4 maroon = rgb(238, 153, 160);
			ImVec4 peach = rgb(245, 169, 127);
			ImVec4 yellow = rgb(238, 212, 159);
			ImVec4 green = rgb(166, 218, 149);
			ImVec4 teal = rgb(139, 213, 202);
			ImVec4 sky = rgb(145, 215, 227);
			ImVec4 sapphire = rgb(125, 196, 228);
			ImVec4 blue = rgb(138, 173, 244);
			ImVec4 lavender = rgb(183, 189, 248);
			ImVec4 text = rgb(202, 211, 245);
			ImVec4 subtext1 = rgb(184, 192, 224);
			ImVec4 subtext0 = rgb(165, 173, 203);
			ImVec4 overlay2 = rgb(147, 154, 183);
			ImVec4 overlay1 = rgb(128, 135, 162);
			ImVec4 overlay0 = rgb(110, 115, 141);
			ImVec4 surface2 = rgb(91, 96, 120);
			ImVec4 surface1 = rgb(73, 77, 100);
			ImVec4 surface0 = rgb(54, 58, 79);
			ImVec4 base_macchiato = rgb(36, 39, 58);
			ImVec4 mantle_macchiato = rgb(30, 32, 48);
			ImVec4 crust_macchiato = rgb(24, 25, 38);
			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_macchiato;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_macchiato;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_macchiato;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_macchiato;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_macchiato;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_macchiato;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_macchiato;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_macchiato;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_macchiato;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_macchiato;  // Unfocused active tab
			return style;
		}


		inline ImGuiStyle CatppuccinMochaStyle() {
			ImGuiStyle style = ImGui::GetStyle();
			auto rgb = [](unsigned char r, unsigned char g, unsigned char b) {
				return ImGui::ColorConvertU32ToFloat4(IM_COL32(r, g, b, 255));
			};

			// Mocha Colors
			ImVec4 rosewater = rgb(245, 224, 220);
			ImVec4 flamingo = rgb(242, 205, 205);
			ImVec4 pink = rgb(245, 194, 231);
			ImVec4 mauve = rgb(203, 166, 247);
			ImVec4 red = rgb(243, 139, 168);
			ImVec4 maroon = rgb(235, 160, 172);
			ImVec4 peach = rgb(250, 179, 135);
			ImVec4 yellow = rgb(249, 226, 175);
			ImVec4 green = rgb(166, 227, 161);
			ImVec4 teal = rgb(148, 226, 213);
			ImVec4 sky = rgb(137, 220, 235);
			ImVec4 sapphire = rgb(116, 199, 236);
			ImVec4 blue = rgb(137, 180, 250);
			ImVec4 lavender = rgb(180, 190, 254);
			ImVec4 text = rgb(205, 214, 244);
			ImVec4 subtext1 = rgb(186, 194, 222);
			ImVec4 subtext0 = rgb(166, 173, 200);
			ImVec4 overlay2 = rgb(147, 153, 178);
			ImVec4 overlay1 = rgb(127, 132, 156);
			ImVec4 overlay0 = rgb(108, 112, 134);
			ImVec4 surface2 = rgb(88, 91, 112);
			ImVec4 surface1 = rgb(69, 71, 90);
			ImVec4 surface0 = rgb(49, 50, 68);
			ImVec4 base_mocha = rgb(30, 30, 46);
			ImVec4 mantle_mocha = rgb(24, 24, 37);
			ImVec4 crust_mocha = rgb(17, 17, 27);

			style.Colors[ImGuiCol_Text] = text;	 // Text color
			style.Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			style.Colors[ImGuiCol_WindowBg] =
				base_mocha;	 // Background color
			style.Colors[ImGuiCol_ChildBg] =
				mantle_mocha;  // Child window background
			style.Colors[ImGuiCol_PopupBg] =
				mantle_mocha;  // Popup background
			style.Colors[ImGuiCol_Border] =
				crust_mocha;  // Border color
			style.Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			style.Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			style.Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			style.Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			style.Colors[ImGuiCol_TitleBg] =
				crust_mocha;  // Title bar background
			style.Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			style.Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			style.Colors[ImGuiCol_MenuBarBg] =
				base_mocha;	 // Menu bar background
			style.Colors[ImGuiCol_ScrollbarBg] =
				mantle_mocha;  // Scrollbar background
			style.Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			style.Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			style.Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			style.Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			style.Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			style.Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			style.Colors[ImGuiCol_Button] = surface2;  // Button color
			style.Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			style.Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			style.Colors[ImGuiCol_Header] = surface1;  // Header color
			style.Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			style.Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			style.Colors[ImGuiCol_Separator] =
				crust_mocha;  // Separator
			style.Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			style.Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			style.Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			style.Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			style.Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			style.Colors[ImGuiCol_Tab] = surface1;	// Tab background
			style.Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			style.Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			style.Colors[ImGuiCol_TabUnfocused] =
				base_mocha;	 // Unfocused tab
			style.Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_mocha;  // Unfocused active tab
			return style;
		}
		inline void ThemeEditor() {
			if (g_ThemeEditorOpen) {
				// TODO: make it actually collapsable WITHOUT IMGUI COMPLAINING ABOUT MISSING END!
				if (ImGui::Begin("Theme editor", &g_ThemeEditorOpen, ImGuiWindowFlags_NoCollapse)) {
					ImGuiStyle& style = ImGui::GetStyle();

					ImVec2 windowSize = ImGui::GetWindowSize();
					float windowWidth = windowSize.x;

					// Calculate the number of col based on the window width
					int numCol = static_cast<int>(windowWidth / 200);  // Each column has a minimum width of 100px
					numCol = ImMax(1, numCol); 

					ImGui::Columns(numCol, nullptr, false);
					for (unsigned char col = 0; col < ImGuiCol_COUNT; ++col) {
						ImGui::PushID(col);
						ImVec4& color = style.Colors[col];

						if (ImGui::ColorEdit4(ImGui::GetStyleColorName(col), (float*)&color)) {
							style.Colors[col] = color;
						}

						if ((col + 1) % numCol == 0) {
							ImGui::NextColumn();
						}

						ImGui::PopID();
					}

					// End the cols
					ImGui::Columns(1);
					
					if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
						g_ThemeEditorOpen = false;
					}

					ImGui::End();
				}
			}
		}
		static CustomTheme g_LastOpenedThemeWhichIsBasicallyAHackToNotReturnADanglingPointerInLoadThemeFromFileFunctionBecauseIWantThisToBeUsable;
		inline CustomTheme* LoadThemeFromFile() {
			using namespace std::string_view_literals;

			nfdchar_t* path = 0;
			nfdresult_t res = NFD_OpenDialog("toml", 0, &path);
			if (res == NFD_ERROR) {
				FUNC_WARN("Failed to open file dialog (error: " << NFD_GetError() << ")");
				return nullptr;
			}
			if (res == NFD_CANCEL) {
				return nullptr;
			}

			toml::parse_result result = toml::parse_file(std::string_view(path));
			if (!result) {
				FUNC_WARN("Failed to parse theme file... it's probably not valid TOML");
				FUNC_WARN("error: " << result.error());
				return nullptr;
			}
			toml::table table = result.table();
			if (!table.contains("meta"sv)) {
				FUNC_WARN("There's no metadata field... please make sure that your theme is valid");
				return nullptr;
			}
			if (!table.contains("colors"sv)) {
				FUNC_WARN("There's no colors in your theme...");
				return nullptr;
			}
			toml::table* meta = table.get("meta"sv)->as_table();
			const char* name = meta->get("name")->as_string()->get().c_str();
			const char* author = meta->get("author")->as_string()->get().c_str();

			toml::table* colors = table.get("colors"sv)->as_table();
			static ImGuiStyle& style = ImGui::GetStyle();
			for (auto node : *colors) {
				for (const auto& [k,v] : *colors) {
					if (!v.is_array()) continue;
					unsigned char parsed = std::atoi(k.str().data());
					toml::array arr = *v.as_array();
					if (arr.size() != 4) continue;
					style.Colors[parsed] = ImVec4(
						arr.get(0)->as_floating_point()->get(),
						arr.get(1)->as_floating_point()->get(),
						arr.get(2)->as_floating_point()->get(),
						arr.get(3)->as_floating_point()->get()
					);
				}

			}
			g_LastOpenedThemeWhichIsBasicallyAHackToNotReturnADanglingPointerInLoadThemeFromFileFunctionBecauseIWantThisToBeUsable = CustomTheme{name, author, &style};
			return &g_LastOpenedThemeWhichIsBasicallyAHackToNotReturnADanglingPointerInLoadThemeFromFileFunctionBecauseIWantThisToBeUsable;
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
						theme.insert("meta"sv, meta);
						toml::table colors = toml::table();
						ImGuiStyle& style = ImGui::GetStyle();
						for (unsigned char i; i < ImGuiCol_COUNT; ++i) {
							std::string is = std::to_string(i);
							std::string_view iv = std::string_view(is);
							colors.insert(iv, toml::array{ style.Colors[i].x, style.Colors[i].y, style.Colors[i].z, style.Colors[i].w });
						}
						theme.insert("colors"sv, colors);

						std::ofstream f(savePath);
						if (!f) {
							FUNC_WARN("Failed to open file...");
							return;
						}
						FUNC_INF("saving current theme to " << savePath << "...");
						f << theme;
						f.close();
					} else if (res == NFD_ERROR) {
						FUNC_WARN("Failed to open save theme dialog: " << NFD_GetError());
					}
				}
				ImGui::End();
			}
		}
		inline void InitThemes() {
			static ImGuiStyle funcdoodleStyle = FuncDoodleStyle();
			g_Themes.at(0).Style = &funcdoodleStyle;
			static ImGuiStyle dark = FuncDoodleStyle();
			ImGui::StyleColorsDark(&dark);
			g_Themes.at(1).Style = &dark;
			static ImGuiStyle light = FuncDoodleStyle();
			ImGui::StyleColorsLight(&light);
			g_Themes.at(2).Style = &light;
			static ImGuiStyle classic = FuncDoodleStyle();
			ImGui::StyleColorsClassic(&classic);
			g_Themes.at(3).Style = &classic;
			static ImGuiStyle catppuccinMocha = CatppuccinMochaStyle();
			g_Themes.at(4).Style = &catppuccinMocha;
			static ImGuiStyle catppuccinMacchiato = CatppuccinMacchiatoStyle();
			g_Themes.at(5).Style = &catppuccinMacchiato;
			static ImGuiStyle catppuccinFrappe = CatppuccinFrappeStyle();
			g_Themes.at(6).Style = &catppuccinFrappe;
			static ImGuiStyle catppuccinLatte = CatppuccinLatteStyle();
			g_Themes.at(7).Style = &catppuccinFrappe;
		}
	}
}
