#include "Keybinds.h"
#include <cstring>
#include <map>

namespace FuncDoodle {
	KeyMask::KeyMask() {
		for (int i = 0; i < KEY_MASK_SIZE; i++) {
			m_Keys[i] = 0;
		}
	}

	KeyMask::KeyMask(ImGuiKey key) {
		for (int i = 0; i < KEY_MASK_SIZE; i++) {
			m_Keys[i] = 0;
		}
		if (key != ImGuiKey_None) {
			int bucket = key / 64;
			m_Keys[bucket] |= (1ull << (key % 64));
		}
	}

	bool KeyMask::IsPressed() const {
		for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; k++) {
			int bucket = k / 64;
			if (m_Keys[bucket] & (1ull << (k % 64))) {
				if (ImGui::IsKeyPressed((ImGuiKey)k))
					return true;
			}
		}
		return false;
	}

	KeyMask::operator char*() {
		static char buf[256] = {};
		buf[0] = '\0';
		bool first = true;
		for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; k++) {
			int bucket = k / 64;
			if (m_Keys[bucket] & (1ull << (k % 64))) {
				if (!first) {
					std::strcat(buf, " | ");
				}
				std::strcat(buf, ImGui::GetKeyName((ImGuiKey)k));
				first = false;
			}
		}
		return buf;
	}

	KeyMask KeyMask::operator|(const KeyMask& other) const {
		KeyMask result;
		for (int i = 0; i < KEY_MASK_SIZE; i++) {
			result.m_Keys[i] = m_Keys[i] | other.m_Keys[i];
		}
		return result;
	}

	Shortcut::operator char*() {
		static char buf[64] = {};
		buf[0] = '\0';
		std::strcat(buf, RequiresCtrl ? "Ctrl + " : "");
		std::strcat(buf, RequiresShift ? "Shift + " : "");
		std::strcat(buf, RequiresSuper ? "Alt + " : "");
		std::strcat(buf, Key);
		return buf;
	}

	bool Shortcut::IsPressed() {
		ImGuiIO& io = ImGui::GetIO();

		return (RequiresCtrl == io.KeyCtrl) && (RequiresShift == io.KeyShift) &&
			   (RequiresSuper == io.KeySuper) && Key.IsPressed();
	}

	KeybindsRegistry::KeybindsRegistry() : m_Reg({}) {}

	Shortcut KeybindsRegistry::Get(const char* id) {
		auto it = m_Reg.find(id);
		if (it != m_Reg.end())
			return it->second;
		return {};
	}

	void KeybindsRegistry::Register(const char* id, Shortcut shortcut) {
		m_Reg[id] = shortcut;
	}
}  // namespace FuncDoodle
