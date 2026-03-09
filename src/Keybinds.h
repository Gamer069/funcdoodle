#pragma once

#include <cstring>
#include <filesystem>
#include <imgui.h>
#include <map>
#include <optional>

namespace FuncDoodle {
	constexpr int KEY_MASK_SIZE = (ImGuiKey_NamedKey_END + 63) / 64;

	class KeyMask {
		public:
			KeyMask();
			KeyMask(ImGuiKey key);
			KeyMask(const KeyMask& other) = default;

			bool IsPressed() const;
			operator char*() const;

			KeyMask operator|(const KeyMask& other) const;

		private:
			unsigned long long m_Keys[KEY_MASK_SIZE];
	};

	struct Shortcut {
		Shortcut();
		Shortcut(const char* str);
		Shortcut(bool requiresCtrl, bool requiresShift, bool requiresSuper, KeyMask key);

		bool RequiresCtrl;
		bool RequiresShift;
		bool RequiresSuper;
		KeyMask Key;

		operator char*() const;
		bool IsPressed();
	};

	struct ShortcutWithUser {
		Shortcut Default;
		std::optional<Shortcut> User;
	};

	class KeybindsRegistry {
		public:
			KeybindsRegistry(std::filesystem::path rootPath);
			~KeybindsRegistry();
			Shortcut Get(const char* id);
			void Register(const char* id, Shortcut shortcut);
			void End();

		private:
			std::map<const char*, ShortcutWithUser> m_Reg;
			std::filesystem::path m_RootPath;

		private:
			void Write();
	};
}  // namespace FuncDoodle
