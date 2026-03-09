#include "Keybinds.h"
#include "exepath.h"
#include "imgui.h"
#include "toml++.h"
#include <cstring>
#include <map>

#include "MacroUtils.h"

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

	KeyMask::operator char*() const {
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

	Shortcut::Shortcut()
		: RequiresCtrl(false), RequiresShift(false), RequiresSuper(false),
		  Key(ImGuiKey_None) {}

	Shortcut::Shortcut(bool requiresCtrl, bool requiresShift, bool requiresSuper, KeyMask key) : RequiresCtrl(requiresCtrl), RequiresShift(requiresShift), RequiresSuper(requiresSuper), Key(key) {}

	Shortcut::Shortcut(const char* str)
		: RequiresCtrl(false), RequiresShift(false), RequiresSuper(false),
		Key(ImGuiKey_None) {
			if (!str || !*str)
				return;

			auto trim = [](std::string& s) {
				while (!s.empty() && std::isspace(s.front()))
					s.erase(s.begin());
				while (!s.empty() && std::isspace(s.back()))
					s.pop_back();
			};

			auto keyFromName = [](const std::string& name) -> ImGuiKey {
				for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; k++) {
					const char* keyName = ImGui::GetKeyName((ImGuiKey)k);
					if (keyName && name == keyName)
						return (ImGuiKey)k;
				}
				return ImGuiKey_None;
			};

			std::string input = str;

			size_t start = 0;
			while (true) {
				size_t pos = input.find('+', start);
				std::string token =
					input.substr(start, pos == std::string::npos ? pos : pos - start);

				trim(token);

				if (token == "Ctrl")
					RequiresCtrl = true;
				else if (token == "Shift")
					RequiresShift = true;
				else if (token == "Cmd" || token == "Super")
					RequiresSuper = true;
				else {
					// Key mask (may contain |)
					KeyMask mask;

					size_t kstart = 0;
					while (true) {
						size_t kpos = token.find('|', kstart);
						std::string keyName =
							token.substr(kstart, kpos == std::string::npos ? kpos
									: kpos - kstart);
						trim(keyName);

						ImGuiKey k = keyFromName(keyName);
						if (k != ImGuiKey_None)
							mask = mask | KeyMask(k);

						if (kpos == std::string::npos)
							break;

						kstart = kpos + 1;
					}

					Key = mask;
				}

				if (pos == std::string::npos)
					break;

				start = pos + 1;
			}
		}

	Shortcut::operator char*() const {
		static char buf[64] = {};
		buf[0] = '\0';
		std::strcat(buf, RequiresCtrl ? "Ctrl + " : "");
		std::strcat(buf, RequiresShift ? "Shift + " : "");
		std::strcat(buf, RequiresSuper ? "Cmd + " : "");
		std::strcat(buf, Key);
		return buf;
	}

	bool Shortcut::IsPressed() {
		ImGuiIO& io = ImGui::GetIO();

		return (RequiresCtrl == io.KeyCtrl) && (RequiresShift == io.KeyShift) &&
			(RequiresSuper == io.KeySuper) && Key.IsPressed();
	}

	KeybindsRegistry::KeybindsRegistry(std::filesystem::path rootPath) : m_Reg({}), m_RootPath(rootPath) {}
	KeybindsRegistry::~KeybindsRegistry() {
		Write();
	}

	Shortcut KeybindsRegistry::Get(const char* id) {
		auto it = m_Reg.find(id);
		if (it != m_Reg.end())
			return it->second.User.value_or(it->second.Default);
		return {};
	}

	void KeybindsRegistry::Register(const char* id, Shortcut shortcut) {
		m_Reg[id] = { .Default = shortcut, .User = std::nullopt };
	}

	void KeybindsRegistry::End() {
		// read toml
		std::filesystem::path keybindsPath = m_RootPath / "keybinds.toml";

		if (!std::filesystem::exists(keybindsPath)) return;

		toml::parse_result res = toml::parse_file(keybindsPath.string());
		toml::table* keybinds = res.get("keybinds")->as_table();
		for (const auto& [k, v] : *keybinds) {
			if (!v.is_string()) {
				FUNC_WARN("keybind not a string - setting to default...");
				continue;
			}
			
			m_Reg[k.str().data()] = ShortcutWithUser(v.as_string()->get().c_str(), std::nullopt);
		}
	}

	void KeybindsRegistry::Write() {
		std::filesystem::path keybindsPath = m_RootPath / "keybinds.toml";

		if (std::filesystem::exists(keybindsPath)) return;

		toml::table root;
		toml::table keybinds;

		for (auto& [id, shortcut] : m_Reg) {
			const Shortcut& s = shortcut.User.value_or(shortcut.Default);
			const char* str = (char*)s;
			keybinds.insert(id, str);
		}

		root.insert("keybinds", keybinds);

		std::ofstream out(keybindsPath);
		out << root;
	}
}  // namespace FuncDoodle
