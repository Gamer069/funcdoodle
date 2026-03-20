#pragma once

#include "imgui.h"
namespace FuncDoodle {
	namespace ImUtil {
		ImGuiKey GetAnyReleasedKey() {
			for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) {
				ImGuiKey key = (ImGuiKey)i;
				if (key == ImGuiKey_LeftCtrl  || key == ImGuiKey_RightCtrl  ||
						key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift ||
						key == ImGuiKey_LeftAlt   || key == ImGuiKey_RightAlt   ||
						key == ImGuiKey_LeftSuper || key == ImGuiKey_RightSuper || key == ImGuiKey_MouseLeft || key == ImGuiKey_MouseRight)
					continue;
				if (ImGui::IsKeyReleased(key))
					return key;
			}
			return ImGuiKey_None;
		}
	}
}
