#include "PopupRegistry.h"

#include "UI/Gui.h"
#include "Anim/Anim.h"

#include <functional>

namespace FuncDoodle {
	void PopupRegistry::Register(std::string_view id) {
		// uhh default params go brr i guess
		m_Popups.emplace(id, PopupEntry{});
	}

	void PopupRegistry::Open(std::string_view id) {
		m_Popups[id].IsOpen = true;
	}

	void PopupRegistry::Close(std::string_view id) {
		m_Popups[id].IsOpen = false;
	}

	void PopupRegistry::Popup(std::string_view humanReadable, std::string_view id, double dt, std::function<void()> popupFn) {
		// sad that i have to use std::string here
		std::string imguiId = FUNC_FMT("{}##{}", humanReadable, id);
		PopupEntry& popup = m_Popups[id];

		if (popup.IsOpen) {
			ImGui::OpenPopup(imguiId.c_str());
			popup.IsOpen = false;
		}

		if (popup.SizeReady && popup.FullSize.x > 0 && popup.Pos.x > 0 && popup.AnimationProgress < 1.0) {
			double t = Anim::Animate(true, 0.2, popup.AnimationProgress, dt, Anim::OutBack);
			float curH = std::max(1.0f, (float)(popup.FullSize.y * t));

			ImVec2 center = {
				popup.Pos.x + popup.FullSize.x / 2.0f,
				popup.Pos.y + curH / 2.0f
			};

			ImGui::SetNextWindowSize({ popup.FullSize.x, curH });

			if (t < 1.0)
				ImGui::SetNextWindowPos(center, ImGuiCond_Always, { 0.5f, 0.5f });
		}

		if (ImGui::BeginPopup(imguiId.c_str(), ImGuiWindowFlags_AlwaysAutoResize)) {
			popupFn();

			if (!popup.SizeReady)
				popup.SizeReady = true;
			else if (popup.FullSize.x < 0) {
				popup.FullSize = ImGui::GetWindowSize();

				// don't know why i need this += 2.0f but i dont care enough to find out
				popup.FullSize.y -= ImGui::GetTextLineHeight();
				popup.FullSize.y += ImGui::GetStyle().ItemSpacing.y / 2.0f;
				popup.FullSize.y += ImGui::GetStyle().WindowPadding.y;
				popup.FullSize.y += 2.0f;

				popup.Pos = ImGui::GetWindowPos();
			} else {
				popup.Pos = ImGui::GetWindowPos();
			}

			ImGui::EndPopup();
		} else if (popup.SizeReady) {
			popup.SizeReady = false;
			popup.FullSize = { -1, -1 };
			popup.AnimationProgress = 0;
		}
	}

	void PopupRegistry::Modal(std::string_view humanReadable, std::string_view id, double dt, std::function<void()> popupFn) {
		// sad that i have to use std::string here
		std::string imguiId = FUNC_FMT("{}##{}", humanReadable, id);
		PopupEntry& popup = m_Popups[id];

		if (popup.IsOpen) {
			ImGui::OpenPopup(imguiId.c_str());
		}

		if (popup.SizeReady && popup.FullSize.x > 0 && popup.Pos.x > 0 && popup.AnimationProgress < 1.0) {
			double t = Anim::Animate(true, 0.2, popup.AnimationProgress, dt, Anim::OutBack);
			float curH = std::max(1.0f, (float)(popup.FullSize.y * t));

			ImVec2 center = {
				popup.Pos.x + popup.FullSize.x / 2.0f,
				popup.Pos.y + curH / 2.0f
			};

			ImGui::SetNextWindowSize({ popup.FullSize.x, curH });

			if (t < 1.0)
				ImGui::SetNextWindowPos(center, ImGuiCond_Always, { 0.5f, 0.5f });
		}


		if (ImGui::BeginPopupModal(imguiId.c_str(), Get(id), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
			popupFn();

			if (!popup.SizeReady)
				popup.SizeReady = true;
			else if (popup.FullSize.x < 0) {
				popup.FullSize = ImGui::GetWindowSize();

				// don't know why i need this += 2.0f but i dont care enough to find out
				popup.FullSize.y -= ImGui::GetTextLineHeight();
				popup.FullSize.y += ImGui::GetStyle().ItemSpacing.y / 2.0f;
				popup.FullSize.y += ImGui::GetStyle().WindowPadding.y;
				popup.FullSize.y += 2.0f;

				popup.Pos = ImGui::GetWindowPos();
			} else {
				popup.Pos = ImGui::GetWindowPos();
			}

			ImGui::EndPopup();
		} else if (popup.SizeReady) {
			popup.SizeReady = false;
			popup.FullSize = { -1, -1 };
			popup.AnimationProgress = 0;
		}
	}

	bool PopupRegistry::IsOpen(std::string_view id) const {
		auto it = m_Popups.find(id);
		if (it != m_Popups.end()) {
			return it->second.IsOpen;
		}
		return false;
	}

	bool* PopupRegistry::Get(std::string_view id) {
		return &m_Popups[id].IsOpen;
	}

	void PopupRegistry::CloseAll() {
		for (auto& [id, popup] : m_Popups) {
			popup.IsOpen = false;
		}
	}

	void PopupRegistry::CloseAllExcept(std::string_view exception) {
		for (auto& [id, popup] : m_Popups) {
			if (id == exception)
				continue;

			popup.IsOpen = false;
		}
	}
}  // namespace FuncDoodle
