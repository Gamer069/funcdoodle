#include "Manager.h"

#include "Core/App.h"
#include "Core/AppSettings.h"

#include "Conf/Constants.h"

#include "Keybinds/KeyHandler.h"
#include "Keybinds/Keybinds.h"

#include "Asset/LoadedAssets.h"
#include "Project/Project.h"

#include "Rendering/FrameRenderer.h"

#include "Tool/ToolManager.h"

#include "UI/Gui.h"
#include "UI/ImUtil.h"

#include "Util/Ptr.h"
#include "imgui.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <utility>

namespace FuncDoodle {
	AnimationManager::AnimationManager(SharedPtr<ProjectFile> proj,
		AssetLoader* assetLoader, SharedPtr<EditorController> editorController,
		KeybindsRegistry& keybinds, AppSettings& appSettings)
		: m_Proj(proj), m_Player(new AnimationPlayer(proj)),
		  m_EditorController(std::move(editorController)),
		  m_AssetLoader(assetLoader), m_Keybinds(keybinds),
		  m_ToolManager(std::make_unique<ToolManager>(keybinds)),
		  m_Settings(appSettings) {
		m_FrameRenderer =
			std::make_unique<FrameRenderer>(nullptr, -1, m_ToolManager.get(),
				m_Player.get(), m_EditorController, m_Settings);
		m_TimelineFrameRenderer =
			std::make_unique<FrameRenderer>(nullptr, -1, m_ToolManager.get(),
				m_Player.get(), m_EditorController, m_Settings);
		m_FrameRenderer->SetUndoByStroke(m_Settings.UndoByStroke);
	}

	AnimationManager::~AnimationManager() = default;

	void AnimationManager::ResetProjectState() {
		m_Proj.reset();
		m_SelectedFrame = 0;
		m_Player->SetPlaying(false);
		m_Player->SetCurFrame(0);
		m_Player->SetProj(nullptr);

		for (FrameRenderer* renderer :
			{m_FrameRenderer.get(), m_TimelineFrameRenderer.get()}) {
			if (!renderer) {
				continue;
			}

			EditorController::CanvasContext* ctx = renderer->GetCtx();
			ctx->Frame = nullptr;
			ctx->PreviousFrame = nullptr;
			ctx->Index = 0;
			ctx->Grid.reset();
			ctx->LastMousePos = ImVec2(-1, -1);
			ctx->LastHoverMousePos = ImVec2(-1, -1);
		}

		if (m_EditorController) {
			m_EditorController->ResetState();
		}
	}

	void AnimationManager::RegisterKeybinds() {
		m_Keybinds.Register("rewind", "Rewind", "Jump to the first frame",
			{false, false, false, ImGuiKey_J});
		m_Keybinds.Register("play", "Play", "Play or pause the animation",
			{false, false, false, ImGuiKey_K});
		m_Keybinds.Register("end", "End", "Jump to the last frame",
			{false, false, false, ImGuiKey_L});

		m_Keybinds.Register("import_before", "Import before",
			"Import frames before the current frame",
			{true, true, false, ImGuiKey_I});
		m_Keybinds.Register("import_after", "Import after",
			"Import frames after the current frame",
			{true, false, false, ImGuiKey_I});
		// false, ImGuiKey_H});
		m_ToolManager->RegisterKeybinds();
	}

	void AnimationManager::RenderFrame() {
		if (m_SelectedFrame >= m_Proj->AnimFrameCount()) {
			m_SelectedFrame = m_Proj->AnimFrameCount() - 1;
		}

		uint64_t curFrameIdx =
			m_Player->Playing() ? m_Player->CurFrame() : m_SelectedFrame;

		const auto frames = m_Proj->AnimFrames();
		if (m_FrameRenderer->GetCtx()->Frame != frames->Get(curFrameIdx))
			m_FrameRenderer->GetCtx()->Frame = frames->Get(curFrameIdx);

		m_FrameRenderer->GetCtx()->Index = curFrameIdx;

		if (curFrameIdx > 0) {
			m_FrameRenderer->GetCtx()->PreviousFrame =
				frames->Get(curFrameIdx - 1);
		}
		m_FrameRenderer->RenderFrame();
	}

	void AnimationManager::RenderTimeline(bool prevEnabled) {
		Application* app = Application::Get();

		if (!m_Proj)
			return;

		RenderFrame();

		if (ImBegin("Timeline", nullptr,
				ImGuiWindowFlags_HorizontalScrollbar |
					ImGuiWindowFlags_NoBackground,
				app)) {
			auto frameWidth = (float)m_Proj->AnimWidth();
			auto frameHeight = (float)m_Proj->AnimHeight();
			float padding = 25.0f;

			// Calculate total width required for all frames
			float totalWidth =
				m_Proj->AnimFrameCount() * (frameWidth + padding);

			// Create a scrollable region
			ImGuiStyle& style = ImGui::GetStyle();
			float childHeight = ImGui::GetWindowHeight() -
								ImGui::GetCursorPosY() - style.WindowPadding.y -
								30.0f;
			childHeight = std::max(childHeight, 0.0f);

			ImGui::BeginChild("FrameScrollRegion",
				ImVec2(ImGui::GetContentRegionAvail().x, childHeight), false,
				ImGuiWindowFlags_HorizontalScrollbar);

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			// Get the initial top-left position
			ImVec2 topLeft = ImGui::GetCursorScreenPos();

			topLeft.y += 10.0f;
			topLeft.x += 10.0f;

			ImVec2 bottomRight =
				ImVec2(topLeft.x + frameWidth, topLeft.y + frameHeight);

			TimelineKeyContext keyContext;
			keyContext.Proj = m_Proj;
			keyContext.Player = m_Player.get();
			keyContext.FrameRenderer = m_FrameRenderer.get();
			keyContext.SelectedFrame = &m_SelectedFrame;
			KeyHandler::HandleTimelineShortcuts(keyContext, m_Keybinds);

			// Render frames
			for (uint64_t i = 0; i < m_Proj->AnimFrameCount(); i++) {
				ImFont* font = ImGui::GetFont();
				bool selected = (m_Player->Playing() && m_Player->CurFrame() == i) ||
					(!m_Player->Playing() && m_SelectedFrame == i);
				float fontSize = selected ? ImGui::GetFontSize() + 4 : ImGui::GetFontSize();


				char frameStr[32];
				snprintf(frameStr, sizeof(frameStr), "%lu", i);

				ImVec2 textSize = ImGui::CalcTextSize(frameStr);


				drawList->AddText(font, fontSize,
					selected
						? ImVec2(topLeft.x + (frameWidth / 2) - (textSize.x / 2), bottomRight.y + 7)
						: ImVec2(topLeft.x + (frameWidth / 2) - (textSize.x / 2), bottomRight.y + 3),
					// constant hell
					IM_COL32(g_MaxColorValue, g_MaxColorValue, g_MaxColorValue, g_AlphaOpaque),
					frameStr);

				EditorController::CanvasContext* ctx = m_TimelineFrameRenderer->GetCtx();
				SharedPtr<LongIndexArray> frames = m_Proj->AnimFrames();

				ctx->Frame = frames->Get(i);


				float width = bottomRight.x - topLeft.x;
				float height = bottomRight.y - topLeft.y;
				float scaleX = width / frameWidth;
				float scaleY = width / frameHeight;

				m_TimelineFrameRenderer->GetCtx()->PixelScale = std::min<float>(scaleX, scaleY);

				m_TimelineFrameRenderer->RenderFramePixels(
					topLeft.x, topLeft.y, ImGui::GetWindowDrawList(), true);

				if (selected) {
					drawList->AddRect(topLeft,
						ImVec2(bottomRight.x, bottomRight.y),
						ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_TextSelectedBg)),
						0.0f, 0, 3.0f);
				}
				ImVec2 mousePos = ImGui::GetMousePos();
				bool isHovered =
					(mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
						mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);

				char menuName[32];	// Make buffer big enough for "frame" +
									// numbers
									// + "menu" + null terminator
				snprintf(menuName, 31, "#frame%ldmenu", i);
				char* menuNamePtr = menuName;

				// Create unique ID for this popup
				if (isHovered && ImGui::IsMouseClicked(1)) {
					ImGui::OpenPopup(menuNamePtr);
				} else if (isHovered && ImGui::IsMouseClicked(0)) {
					m_SelectedFrame = i;
				}

				if (ImGui::BeginPopup(menuNamePtr)) {
					Shortcut deleteKey = m_Keybinds.Get("delete_frame");
					Shortcut insertBeforeKey = m_Keybinds.Get("insert_before");
					Shortcut insertAfterKey = m_Keybinds.Get("insert_after");
					Shortcut moveForwardKey = m_Keybinds.Get("move_forward");
					Shortcut moveBackwardKey = m_Keybinds.Get("move_backward");
					Shortcut copyKey = m_Keybinds.Get("copy_frame");
					Shortcut pasteBeforeKey = m_Keybinds.Get("paste_before");
					Shortcut pasteAfterKey = m_Keybinds.Get("paste_after");

					// deletion
					if (ImGui::MenuItem("Delete", deleteKey)) {
						if (m_Proj->AnimFrameCount() != 1) {
							Frame deletedFrame =
								*m_Proj->AnimFrames()->Get(m_SelectedFrame);

							m_Proj->AnimFrames()->Remove(m_SelectedFrame);

							DeleteFrameAction action(
								m_SelectedFrame, &deletedFrame, m_Proj);

							m_Proj->PushUndoable(action);
							m_Proj->AnimFrames()->Remove(i);
						}
					}

					// insertion
					if (ImGui::MenuItem("Insert before", insertBeforeKey)) {
						m_Proj->AnimFrames()->InsertBeforeEmpty(
							m_SelectedFrame);
						m_SelectedFrame++;
						InsertFrameAction action(m_SelectedFrame - 1, m_Proj);
						m_Proj->PushUndoable(action);
					}
					if (ImGui::MenuItem("Insert after", insertAfterKey)) {
						m_Proj->AnimFrames()->InsertAfterEmpty(m_SelectedFrame);
						InsertFrameAction action(m_SelectedFrame + 1, m_Proj);
						m_Proj->PushUndoable(action);
					}

					// moving
					if (ImGui::MenuItem("Move forward", moveForwardKey)) {
						m_Proj->AnimFrames()->MoveForward(i);
					}
					if (ImGui::MenuItem("Move backward", moveBackwardKey)) {
						m_Proj->AnimFrames()->MoveBackward(i);
					}

					// copy
					if (ImGui::MenuItem("Copy", copyKey)) {
						m_Proj->AnimFrames()->Get(i)->CopyToClipboard();
					}

					// paste
					if (ImGui::MenuItem("Paste before", pasteBeforeKey)) {
						Frame* frame = Frame::PastedFrame();
						m_Proj->AnimFrames()->InsertBefore(i, frame);
					}
					if (ImGui::MenuItem("Paste after", pasteAfterKey)) {
						Frame* frame = Frame::PastedFrame();
						m_Proj->AnimFrames()->InsertAfter(i, frame);
					}

					ImGui::EndPopup();
				}

				topLeft.x += frameWidth + padding;
				bottomRight.x += frameWidth + padding;
			}

			// Ensure the scroll region size is based on total width of all
			// frames
			ImGui::Dummy(ImVec2(totalWidth - 25, frameHeight));

			{
				const ImVec2 addButtonSize(20, 20);
				ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
				ImGui::SetCursorPos(
					ImVec2(ImGui::GetScrollX() + contentMax.x - 40 -
							   ImGui::GetStyle().FramePadding.x,
						ImGui::GetScrollY() + contentMax.y - 40 -
							ImGui::GetStyle().FramePadding.y));

				if (ImGui::ImageButton("##add",
						(ImTextureID)(intptr_t)s_AddTexId, addButtonSize)) {
					m_Proj->AnimFrames()->InsertAfterEmpty(m_SelectedFrame);
					InsertFrameAction action(m_SelectedFrame + 1, m_Proj);
					m_Proj->PushUndoable(action);
				}
			}

			ImGui::EndChild();
		}
		ImEnd();

		m_ToolManager->RenderTools();
	}

	void AnimationManager::RenderControls() {
		Application* app = Application::Get();

		if (ImBegin("Controls", nullptr, 0, app)) {
			if (ImGui::ImageButton("rewind",
					(ImTextureID)(intptr_t)s_RewindTexId, ImVec2(20, 20)) ||
				(m_Keybinds.Get("rewind").IsPressed() &&
					!ImGui::IsAnyItemActive())) {
				m_SelectedFrame = 0;
				m_Player->Rewind();
			}

			ImGui::SameLine();

			if (ImGui::ImageButton("togglePlay",
					m_Player->Playing() ? (ImTextureID)(intptr_t)s_PauseTexId
										: (ImTextureID)(intptr_t)s_PlayTexId,
					ImVec2(20, 20)) ||
				(m_Keybinds.Get("play").IsPressed() &&
					!ImGui::IsAnyItemActive())) {
				m_Player->SetPlaying(!m_Player->Playing());
			}

			ImGui::SameLine();

			if (ImGui::ImageButton(
					"end", (ImTextureID)(intptr_t)s_EndTexId, ImVec2(20, 20)) ||
				(m_Keybinds.Get("end").IsPressed() &&
					!ImGui::IsAnyItemActive())) {
				m_SelectedFrame = m_Proj->AnimFrameCount() - 1;
				m_Player->End();
			}
		}

		ImEnd();
	}

	void AnimationManager::RenderLogs() {
		Application* app = Application::Get();

		if (ImBegin("Logs", nullptr, 0, app)) {
			const auto logColor = [](const char* s) -> ImVec4 {
				if (!s) {
					return {1.0f, 1.0f, 1.0f, 1.0f};
				}
				if (std::strstr(s, "[Error]") || std::strstr(s, "[FATAL]")) {
					return {1.0f, 0.35f, 0.35f, 1.0f};
				}
				if (std::strstr(s, "[Warn]")) {
					return {1.0f, 0.75f, 0.25f, 1.0f};
				}
				if (std::strstr(s, "[Debug]")) {
					return {0.45f, 0.85f, 1.0f, 1.0f};
				}
				if (std::strstr(s, "[Info]")) {
					return {0.55f, 0.75f, 1.0f, 1.0f};
				}
				if (std::strstr(s, "[Note]")) {
					return {0.7f, 0.7f, 0.7f, 1.0f};
				}
				return {1.0f, 1.0f, 1.0f, 1.0f};
			};

			if (ImGui::Button("Clear")) {
				s_Logs.clear();
			}

			ImGui::SameLine();

			if (ImGui::Button("Copy")) {
				ImGui::LogToClipboard();

				// don't really like that i have to use std::string for logs,
				// but i think thats necessary
				for (const std::string& str : s_Logs) {
					ImGui::LogText("%s\n", str.c_str() ? str.c_str() : "");
				}

				ImGui::LogFinish();
			}

			ImGui::PushStyleColor(
				ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

			ImGui::BeginChild("##logscroll", ImVec2(-1, -1), false,
				ImGuiWindowFlags_HorizontalScrollbar);

			for (const std::string& str : s_Logs) {
				ImGui::TextColored(logColor(str.c_str()), "%s",
					str.c_str() ? str.c_str() : "");
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}

		ImEnd();
	}
}  // namespace FuncDoodle
