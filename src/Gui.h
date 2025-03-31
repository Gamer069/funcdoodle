#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <nfd.h>
#include <ostream>

inline std::ostream& operator<<(std::ostream& os, const ImGuiStyle& style) {
	os << "ImGuiStyle { "
	   << "Alpha: " << style.Alpha << ", "
	   << "DisabledAlpha: " << style.DisabledAlpha << ", "
	   << "WindowPadding: (" << style.WindowPadding.x << ", "
	   << style.WindowPadding.y << "), "
	   << "WindowRounding: " << style.WindowRounding << ", "
	   << "WindowBorderSize: " << style.WindowBorderSize << ", "
	   << "FramePadding: (" << style.FramePadding.x << ", "
	   << style.FramePadding.y << "), "
	   << "FrameRounding: " << style.FrameRounding << ", "
	   << "ItemSpacing: (" << style.ItemSpacing.x << ", " << style.ItemSpacing.y
	   << "), "
	   << "ScrollbarSize: " << style.ScrollbarSize << ", "
	   << "ScrollbarRounding: " << style.ScrollbarRounding << ", "
	   << "GrabMinSize: " << style.GrabMinSize << ", "
	   << "GrabRounding: " << style.GrabRounding << ", "
	   << "TabRounding: " << style.TabRounding << " }\n";

	// Print colors
	os << "Colors:\n";
	for (int i = 0; i < ImGuiCol_COUNT; ++i) {
		const ImVec4& col = style.Colors[i];
		os << "  [" << ImGuiCol_(i) << "] (" << col.x << ", " << col.y << ", "
		   << col.z << ", " << col.w << ")\n";
	}

	return os;
}
