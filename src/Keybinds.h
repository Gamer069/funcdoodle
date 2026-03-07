#pragma once

#include <cstring>
#include <imgui.h>
#include <map>

namespace FuncDoodle {
	constexpr int KEY_MASK_SIZE = (ImGuiKey_NamedKey_END + 63) / 64;

	class KeyMask {
		public:
			KeyMask();
			KeyMask(ImGuiKey key);
			KeyMask(const KeyMask& other) = default;

			bool IsPressed() const;
			operator char*();

			KeyMask operator|(const KeyMask& other) const;

		private:
			unsigned long long m_Keys[KEY_MASK_SIZE];
	};

	struct Shortcut {
			bool RequiresCtrl;
			bool RequiresShift;
			bool RequiresSuper;
			KeyMask Key;

			operator char*();
			bool IsPressed();
	};

	struct StrCmp {
			bool operator()(const char* a, const char* b) const {
				return std::strcmp(a, b) < 0;
			}
	};

	class KeybindsRegistry {
		public:
			KeybindsRegistry();
			Shortcut Get(const char* id);
			void Register(const char* id, Shortcut shortcut);

		private:
			// id -> shortcut map
			std::map<const char*, Shortcut, StrCmp> m_Reg;
	};
}  // namespace FuncDoodle
