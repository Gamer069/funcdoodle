#pragma once

#include "Gui.h"

namespace FuncDoodle {
	namespace Themes {
		static void FuncDoodleStyle() {
			ImGui::StyleColorsDark();

			ImGuiStyle* style = &ImGui::GetStyle();
			ImGuiIO& io = ImGui::GetIO();
			style->Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
			style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
			style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
			style->WindowRounding = 10.0f;
			style->FrameRounding = 5.0f;
			style->PopupRounding = 12.0f;
			style->ScrollbarRounding = 10.0f;
			style->GrabRounding = 6.0f;
			style->TabRounding = 12.0f;
			style->ChildRounding = 12.0f;
			style->WindowPadding = ImVec2(10, 10);
			style->FramePadding = ImVec2(8, 8);
			style->ItemSpacing = ImVec2(10, 10);
			style->IndentSpacing = 20.0f;
			style->ScrollbarSize = 16.0f;
			ImVec4* colors = style->Colors;
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
			style->Alpha = 1.0f;  // Fully opaque
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				style->WindowRounding = 1.0f;
			}
		}

		static void CatppuccinLatteStyle() {
			ImGui::StyleColorsDark();
			FuncDoodleStyle();
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
			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_latte;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_latte;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_latte;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_latte;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_latte;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_latte;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_latte;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_latte;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_latte;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_latte;  // Unfocused active tab
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_latte;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_latte;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_latte;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_latte;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_latte;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_latte;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_latte;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_latte;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_latte;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_latte;  // Unfocused active tab
		}

		static void CatppuccinFrappeStyle() {
			ImGui::StyleColorsDark();
			FuncDoodleStyle();
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
			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_frappe;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_frappe;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_frappe;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_frappe;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_frappe;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_frappe;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_frappe;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_frappe;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_frappe;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_frappe;  // Unfocused active tab
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_frappe;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_frappe;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_frappe;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_frappe;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_frappe;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_frappe;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_frappe;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_frappe;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_frappe;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_frappe;  // Unfocused active tab
		}

		static void CatppuccinMacchiatoStyle() {
			ImGui::StyleColorsDark();
			FuncDoodleStyle();
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
			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_macchiato;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_macchiato;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_macchiato;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_macchiato;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_macchiato;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_macchiato;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_macchiato;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_macchiato;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_macchiato;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_macchiato;  // Unfocused active tab
		}


		static void CatppuccinMochaStyle() {
			ImGui::StyleColorsDark();
			FuncDoodleStyle();
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

			ImGui::GetStyle().Colors[ImGuiCol_Text] = text;	 // Text color
			ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] =
				subtext1;  // Disabled text
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg] =
				base_mocha;	 // Background color
			ImGui::GetStyle().Colors[ImGuiCol_ChildBg] =
				mantle_mocha;  // Child window background
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg] =
				mantle_mocha;  // Popup background
			ImGui::GetStyle().Colors[ImGuiCol_Border] =
				crust_mocha;  // Border color
			ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] =
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f);	 // No shadow
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg] =
				surface1;  // Frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] =
				surface2;  // Hovered frame background
			ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] =
				surface0;  // Active frame background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
				crust_mocha;  // Title bar background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] =
				surface1;  // Active title background
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] =
				surface2;  // Collapsed title background
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] =
				base_mocha;	 // Menu bar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] =
				mantle_mocha;  // Scrollbar background
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] =
				overlay1;  // Scrollbar grab
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] =
				overlay0;  // Scrollbar grab hovered
			ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] =
				surface0;  // Scrollbar grab active
			ImGui::GetStyle().Colors[ImGuiCol_CheckMark] =
				green;	// Checkmark color
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] =
				overlay1;  // Slider grab
			ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] =
				overlay0;  // Active slider grab
			ImGui::GetStyle().Colors[ImGuiCol_Button] = surface2;  // Button color
			ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] =
				surface1;  // Button hover
			ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] =
				surface0;										   // Active button
			ImGui::GetStyle().Colors[ImGuiCol_Header] = surface1;  // Header color
			ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
				surface2;  // Header hover
			ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] =
				surface0;  // Active header
			ImGui::GetStyle().Colors[ImGuiCol_Separator] =
				crust_mocha;  // Separator
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
				overlay1;  // Separator hover
			ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] =
				overlay0;  // Active separator
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] =
				overlay1;  // Resize grip
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] =
				overlay0;  // Resize grip hover
			ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] =
				surface0;  // Active resize grip
			ImGui::GetStyle().Colors[ImGuiCol_Tab] = surface1;	// Tab background
			ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = surface2;  // Tab hover
			ImGui::GetStyle().Colors[ImGuiCol_TabActive] = surface0;   // Active tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] =
				base_mocha;	 // Unfocused tab
			ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
				mantle_mocha;  // Unfocused active tab
		}
	}
}
